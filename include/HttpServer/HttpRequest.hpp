#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <HttpServer/Advanced.hpp>

namespace http = boost::beast::http;

class HttpRequest {
public:
    explicit HttpRequest(const http::request<http::string_body>& req)
        : method_(req.method()), target_(std::string(req.target())), body_(req.body()) {}

    http::verb method() const { return method_; }
    std::string target() const { return target_; }
    std::string body() const { return body_; }

private:
    http::verb method_;
    std::string target_;
    std::string body_;
};

#endif // HTTP_REQUEST_HPP
