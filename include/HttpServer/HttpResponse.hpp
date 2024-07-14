#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <HttpServer/Advanced.hpp>

namespace http = boost::beast::http;

class HttpResponse {
public:
    explicit HttpResponse(http::status status, const std::string& body)
        : status_(status), body_(body) {}

    http::status status() const { return status_; }
    std::string body() const { return body_; }

private:
    http::status status_;
    std::string body_;
};

#endif // HTTPRESPONSE_HPP
