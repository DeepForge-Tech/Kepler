#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <DatabaseAPI/Advanced.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <json/json.h>

namespace http = boost::beast::http;

namespace Kepler
{
    class HttpClient
    {
    public:
        template <typename DataType>
        boost::asio::awaitable<Json::Value> sendPostRequest(const DataType &urlParts,const DataType &data);

        // template <typename DataType>
        HttpClient &setConnectionData(DB::HashedDatabaseValues &connection_data);
        HttpClient &setUrl(const char *connection_url);
        HttpClient &setJsonQueryStr(const char *json_query_str);
        HttpClient &setJsonDataStr(const char *json_data_str);

    protected:
        boost::asio::awaitable<DB::DefaultReturnType> getUrlParts(const std::string &url);

    private:
        std::string connectionUrl;
        std::string jsonQueryStr;
        std::string jsonDataStr;
        std::unordered_map<std::string, std::string> connectionData;
    };
}
#endif