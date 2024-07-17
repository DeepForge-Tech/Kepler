#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <HttpServer/Advanced.hpp>



namespace Kepler
{
    class HttpResponse {
    public:
        explicit HttpResponse(http::status status = http::status::ok, const std::string& body = "")
            : status_(status), body_(body) {}

        http::status status() const { return status_; }
        std::string body() const { return body_; }
        int result_int() const { return static_cast<int>(status_); }

        std::unordered_map<std::string, std::string> headers() const {
            std::unordered_map<std::string, std::string> result;
            for (const auto& field : headers_) {
                result[field.name_string()] = field.value();
            }
            return result;
        }

        void set_header(const std::string& field, const std::string& value) {
            headers_.set(field, value);
        }

        void set_status(http::status status) {
            status_ = status;
        }

        void set_body(const std::string& body) {
            body_ = body;
        }

    private:
        http::status status_;
        std::string body_;
        http::fields headers_;
    };
}

#endif // HTTPRESPONSE_HPP
