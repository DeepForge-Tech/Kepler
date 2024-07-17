#ifndef SESSION_HPP
#define SESSION_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/config.hpp>
#include <memory>
#include <HttpsServer/Router.hpp>
#include <iostream>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Represents an established connection and handles the communication with the client.
class Session : public std::enable_shared_from_this<Session> {
public:
    // Constructor for HTTPS connections.
    Session(tcp::socket socket, ssl::context& ssl_context, const Router& router)
        : stream_(std::move(socket)), ssl_stream_(stream_, ssl_context), router_(router), is_ssl_(true) {}

    // Start the asynchronous operation for the session.
    void start() {
        if (is_ssl_) {
            // Perform SSL handshake
            do_ssl_handshake();
        } else {
            // Start reading request
            do_read();
        }
    }

private:
    // Handle SSL handshake.
    void do_ssl_handshake() {
        auto self = shared_from_this();
        ssl_stream_.async_handshake(ssl::stream_base::server,
            [self](beast::error_code ec) {
                if (!ec) {
                    self->do_read();
                } else {
                    self->fail(ec, "ssl_handshake");
                }
            });
    }

    // Start reading from the socket.
    void do_read() {
        auto self = shared_from_this();
        if (is_ssl_) {
            http::async_read(ssl_stream_, buffer_, request_,
                [self](beast::error_code ec, std::size_t bytes_transferred) {
                    boost::ignore_unused(bytes_transferred);
                    if (!ec) {
                        self->handle_request();
                    } else {
                        self->fail(ec, "read");
                    }
                });
        } else {
            http::async_read(stream_, buffer_, request_,
                [self](beast::error_code ec, std::size_t bytes_transferred) {
                    boost::ignore_unused(bytes_transferred);
                    if (!ec) {
                        self->handle_request();
                    } else {
                        self->fail(ec, "read");
                    }
                });
        }
    }

    // Handle the HTTP request.
    void handle_request() {
        try {
            // Create a simple response
            http::response<http::string_body> res{
                http::status::ok, request_.version()};
            res.set(http::field::server, "Boost.Beast/268");
            res.set(http::field::content_type, "text/html");
            res.keep_alive(request_.keep_alive());
            res.body() = "Hello, world!";
            res.prepare_payload();

            // Send the response
            auto self = shared_from_this();
            if (is_ssl_) {
                http::async_write(ssl_stream_, res,
                    [self](beast::error_code ec, std::size_t bytes_transferred) {
                        boost::ignore_unused(bytes_transferred);
                        if (ec) {
                            self->fail(ec, "write");
                        } else {
                            // Gracefully close the socket
                            self->do_close();
                        }
                    });
            } else {
                http::async_write(stream_, res,
                    [self](beast::error_code ec, std::size_t bytes_transferred) {
                        boost::ignore_unused(bytes_transferred);
                        if (ec) {
                            self->fail(ec, "write");
                        } else {
                            // Gracefully close the socket
                            self->do_close();
                        }
                    });
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in handle_request: " << e.what() << "\n";
        } catch (...) {
            std::cerr << "Unknown exception in handle_request\n";
        }
    }

    // Close the socket connection.
    void do_close() {
        auto self = shared_from_this();
        if (is_ssl_) {
            ssl_stream_.async_shutdown(
                [self](beast::error_code ec) {
                    self->fail(ec, "ssl_shutdown");
                });
        } else {
            stream_.socket().shutdown(tcp::socket::shutdown_send);
        }
    }

    // Report a failure
    void fail(beast::error_code ec, char const* what) {
        if (ec == net::error::operation_aborted) {
            return;
        }
        std::cerr << what << ": " << ec.message() << "\n";
    }

    beast::tcp_stream stream_;
    beast::ssl_stream<beast::tcp_stream&> ssl_stream_;
    beast::flat_buffer buffer_;  // (Must persist between reads)
    http::request<http::string_body> request_;
    const Router& router_;
    bool is_ssl_;
};

#endif // SESSION_HPP
