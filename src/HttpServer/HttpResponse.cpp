#include <Kepler/HttpServer/HttpResponse.hpp>

std::unordered_map<std::string, std::string> Kepler::HttpResponse::headers() const
{
    std::unordered_map<std::string, std::string> result;
    for (const auto &field : headers_)
    {
        result[field.name_string()] = field.value();
    }
    return result;
}

void Kepler::HttpResponse::set_header(const std::string &field, const std::string &value)
{
    headers_.set(field, value);
}

void Kepler::HttpResponse::set_status(http::status status)
{
    status_ = status;
}
void Kepler::HttpResponse::set_body(const std::string &body)
{
    body_ = body;
}