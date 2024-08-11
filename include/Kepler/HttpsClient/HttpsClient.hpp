#ifndef HTTPS_CLIENT_HPP
#define HTTPS_CLIENT_HPP

#include <Kepler/General.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <sstream>

namespace Kepler
{
    class HttpsClient
    {
    public:
        template <typename DataType>
        boost::asio::awaitable<Json::Value> sendPostRequest(DataType &urlParts, DataType &data);

        // template <typename DataType>
        HttpsClient& setConnectionData(HashMap &connection_data);
        HttpsClient& setUrl(const char *connection_url);
        HttpsClient& setJsonQueryStr(const char *json_query_str);
        HttpsClient& setJsonDataStr(const char *json_data_str);

    protected:
    private:
        std::string connectionUrl;
        std::string jsonQueryStr;
        std::string jsonDataStr;
        HashMap connectionData;
    };
}

#endif