#ifndef WEBCLIENT_HPP
#define WEBCLIENT_HPP

#include <Kepler/General.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <Kepler/HttpClient/HttpClient.hpp>
#include <Kepler/HttpsClient/HttpsClient.hpp>
#include <memory>
#include <regex>

namespace Kepler
{
    class WebClient
    {
    public:
        template <typename DataType>
        cppcoro::task<Json::Value> sendPostRequest(const std::string &url, DataType &data);
        WebClient()
        {
            http_client_ptr = std::make_unique<HttpClient>();
            https_client_ptr = std::make_unique<HttpsClient>();
            http_client = http_client_ptr.get();
            https_client = https_client_ptr.get();
        }

    protected:
        cppcoro::task<HashMap> getUrlParts(const std::string &url);

    private:
        std::unique_ptr<HttpClient> http_client_ptr;
        std::unique_ptr<HttpsClient> https_client_ptr;
        HttpClient *http_client;
        HttpsClient *https_client;
    };
}

#endif