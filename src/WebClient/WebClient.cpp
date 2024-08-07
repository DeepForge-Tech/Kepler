#include <Kepler/WebClient/WebClient.hpp>

cppcoro::task<DB::DefaultReturnType> Kepler::WebClient::getUrlParts(const std::string &url)
{
    // std::string url = "https://34.56.255.123/execute";
    DB::HashedDatabaseValues urlParts;
    std::regex url_regex(R"((http|https)://([^:/]+):?(\d*)(/.*)?)");
    std::smatch url_match_result;
    std::string scheme;
    std::string port;
    try
    {
        if (std::regex_match(url, url_match_result, url_regex))
        {
            scheme = url_match_result[1];
            port = url_match_result[3];
            urlParts.insert(std::make_pair("scheme", scheme));
            if (port.empty())
            {
                port = (scheme == "https") ? "443" : "80";
            }
            urlParts.insert(std::make_pair("port", port));
            urlParts.insert(std::make_pair("host", url_match_result[2]));
            urlParts.insert(std::make_pair("path", url_match_result[4]));
        }
    }
    catch(std::exception &error)
    {
        throw std::runtime_error(fmt::format("HttpClient.getUrlParts.{}", error.what()));
    }
    co_return urlParts;
}