#include <HttpServer/HttpRequest.hpp>

std::unordered_map<std::string, std::string> Kepler::HttpRequest::headers() const
{
    std::unordered_map<std::string, std::string> result;
    for (const auto &field : headers_)
    {
        result[field.name_string()] = field.value();
    }
    return result;
}