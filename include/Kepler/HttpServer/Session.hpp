#ifndef SESSION_HPP
#define SESSION_HPP

#include <Kepler/HttpServer/HttpRequest.hpp>
#include <Kepler/HttpServer/HttpResponse.hpp>
#include <Kepler/HttpServer/Router.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
// namespace json = boost::json;
using tcp = asio::ip::tcp;

namespace Kepler
{
    class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(tcp::socket socket, const Router& router)
            : socket(std::move(socket)), router(router) {}

        void start();

    private:
        void do_read();

        void setHeaders(http::response<http::string_body> &response, std::unordered_map<std::string,std::string> headers);

        void send_response(const HttpResponse& res);

        tcp::socket socket;
        beast::flat_buffer buffer;
        http::request<http::string_body> request;
        http::response<http::string_body> response;
        const Router& router;
    };
}

#endif // SESSION_HPP