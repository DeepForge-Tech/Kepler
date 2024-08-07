#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <Kepler/HttpServer/Advanced.hpp>

namespace http = boost::beast::http;

namespace Kepler
{
    class HttpRequest {
    public:
        explicit HttpRequest(const http::request<http::string_body>& req)
            : method_(req.method()), target_(std::string(req.target())), body_(req.body()), headers_(req.base()) {}

        http::verb method() const { return method_; }
        std::string target() const { return target_; }
        std::string body() const { return body_; }

        std::unordered_map<std::string, std::string> headers() const;

    private:
        http::verb method_;
        std::string target_;
        std::string body_;
        http::fields headers_;
    };
}

#endif // HTTP_REQUEST_HPP
