#ifndef SESSION_HPP
#define SESSION_HPP

#include <HttpServer/HttpRequest.hpp>
#include <HttpServer/HttpResponse.hpp>
#include <HttpServer/Router.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using tcp = asio::ip::tcp;

namespace Kepler
{
    class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(tcp::socket socket, const Router& router)
            : socket(std::move(socket)), router(router) {}

        void start() { do_read(); }

    private:
        void do_read() {
            auto self(shared_from_this());
            http::async_read(socket, buffer, request,
                [this, self](beast::error_code ec, std::size_t bytes_transferred) {
                    boost::ignore_unused(bytes_transferred);
                    if (!ec) {
                        HttpRequest req(request);
                        HttpResponse res = router.route(req);

                        send_response(res);
                    }
                });
        }

        void send_response(const HttpResponse& res) {
            auto self(shared_from_this());
            response.result(res.status());
            response.body() = res.body();
            response.prepare_payload();
            http::async_write(socket, response,
                [this, self](beast::error_code ec, std::size_t) {
                    socket.shutdown(tcp::socket::shutdown_send, ec);
                });
        }

        tcp::socket socket;
        beast::flat_buffer buffer;
        http::request<http::string_body> request;
        http::response<http::string_body> response;
        const Router& router;
    };
}

#endif // SESSION_HPP