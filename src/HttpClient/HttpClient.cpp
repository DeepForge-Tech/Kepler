#include <DatabaseAPI/HttpClient.hpp>

template <typename DataType>
boost::asio::awaitable<Json::Value> HttpClient::sendPostRequest(const DataType &data)
{
    try
    {
        auto executor = co_await boost::asio::this_coro::executor;
        boost::system::error_code ec;
        std::string readBuffer;
        Json::Value jsonResponse;
        Json::Value jsonData;
        std::string dataStr;
        Json::CharReaderBuilder reader;
        Json::StreamWriterBuilder writer;
        std::string errs;
        uint16_t http_code;
        DB::DefaultReturnType urlParts;
        // Prepare JSON data

        if (!connectionData.empty())
        {
            for (const auto &element : connectionData)
            {
                jsonData[element.first] = element.second;
            }
        }

        for (const auto &element : data)
        {
            jsonData[element.first] = element.second;
        }

        dataStr = Json::writeString(writer, jsonData);

        boost::asio::ip::tcp::resolver resolver(executor);
        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        boost::asio::ip::tcp::socket socket(executor);

        urlParts = co_await getUrlParts(connectionUrl);

        const auto results = co_await resolver.async_resolve(urlParts["host"], urlParts["port"], boost::asio::use_awaitable);

        co_await boost::asio::async_connect(socket, results, boost::asio::use_awaitable);

        // Create HTTP request
        http::request<http::string_body> req{http::verb::post, urlParts["path"], 11};
        req.set(http::field::host, fmt::format("{}:{}", urlParts["host"], urlParts["port"]));
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/json");
        req.body() = dataStr;
        req.prepare_payload();

        // Send HTTP request
        co_await http::async_write(socket, req, boost::asio::use_awaitable);

        // Receive HTTP response
        co_await http::async_read(socket, buffer, res, boost::asio::use_awaitable);

        http_code = res.result_int();
        readBuffer = res.body();

        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

        // Parse JSON response
        std::istringstream ss(readBuffer);
        if (!Json::parseFromStream(reader, ss, &jsonResponse, &errs))
        {
            throw std::runtime_error(fmt::format("JSON parse error: {}", errs));
        }

        // Handle HTTP response codes
        switch (http_code)
        {
        case 404:
            throw std::runtime_error(jsonData.isMember("message") ? fmt::format("404 Page Not Found.{}", jsonData["message"].asString()) : "404 Page Not Found");
        case 400:
            throw std::runtime_error(jsonData.isMember("message") ? fmt::format("400 Bad Request.{}", jsonData["message"].asString()) : "400 Bad Request");
        case 500:
            throw std::runtime_error(jsonData.isMember("message") ? fmt::format("500 Internal Server Error.{}", jsonData["message"].asString()) : "500 Internal Server Error");
        case 401:
            throw std::runtime_error(jsonData.isMember("message") ? fmt::format("401 Unauthorized.{}", jsonData["message"].asString()) : "401 Unauthorized");
        case 403:
            throw std::runtime_error(jsonData.isMember("message") ? fmt::format("403 Forbidden.{}", jsonData["message"].asString()) : "403 Forbidden");
        case 405:
            throw std::runtime_error(jsonData.isMember("message") ? fmt::format("405 Method Not Allowed.{}", jsonData["message"].asString()) : "405 Method Not Allowed");
        case 408:
            throw std::runtime_error(jsonData.isMember("message") ? fmt::format("408 Request Timeout.{}", jsonData["message"].asString()) : "408 Request Timeout");
        default:
            break;
        }
        co_return jsonResponse;
    }
    catch (std::exception &error)
    {
        throw std::runtime_error(fmt::format("HttpClient.sendPostRequest.{}", error.what()));
    }
}

// template <typename DataType>
HttpClient &HttpClient::setConnectionData(DB::HashedDatabaseValues &connection_data)
{
    connectionData = std::move(connection_data);
    return *this;
}

HttpClient &HttpClient::setUrl(const char *connection_url)
{
    connectionUrl = std::move(connection_url);
    return *this;
}

HttpClient &HttpClient::setJsonQueryStr(const char *json_query_str)
{
    jsonQueryStr = std::move(json_query_str);
    return *this;
}

HttpClient &HttpClient::setJsonDataStr(const char *json_data_str)
{
    jsonDataStr = std::move(json_data_str);
    return *this;
}

boost::asio::awaitable<DB::DefaultReturnType> HttpClient::getUrlParts(const std::string &url)
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

template boost::asio::awaitable<Json::Value> HttpClient::sendPostRequest<DB::HashedDatabaseValues>(const DB::HashedDatabaseValues &);
template boost::asio::awaitable<Json::Value> HttpClient::sendPostRequest<DB::DatabaseValues>(const DB::DatabaseValues &);

// template HttpClient &HttpClient::setConnectionData<DB::HashedDatabaseValues>(DB::HashedDatabaseValues &);
// template HttpClient &HttpClient::setConnectionData<DB::DatabaseValues>(DB::DatabaseValues &);