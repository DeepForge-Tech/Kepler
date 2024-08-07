#include <Kepler/HttpServer/HttpServer.hpp>
#include <Kepler/HttpServer/Router.hpp>
#include <boost/json.hpp>
#include <iostream>

namespace json = boost::json;

void handle_root(const Kepler::HttpRequest &req, Kepler::HttpResponse &res)
{
    std::string body = "<html><body><h1>Welcome to the HTTP Server</h1></body></html>";
    res.set_body(body);
}

void handle_json(const Kepler::HttpRequest &req, Kepler::HttpResponse &res)
{
    json::value jv = {{"message", "Hello, JSON!"}};
    res.set_body(json::serialize(jv));
}

void handle_post(const Kepler::HttpRequest &req, Kepler::HttpResponse &res)
{
    json::value jv = json::parse(req.body());
    res.set_body(json::serialize(jv));
}

void handle_hello(const Kepler::HttpRequest &req, Kepler::HttpResponse &res)
{
    res.set_body("Hello, World!");
}

int main(int argc, char *argv[])
{
    try
    {
        uint16_t port = argc >= 2 ? std::atoi(argv[1]) : 80;
        asio::io_context ioc;

        Kepler::Router router;
        router.get("/", handle_root);
        router.get("/json", handle_json);
        router.post("/post", handle_post);
        router.get("/hello", handle_hello);

        Kepler::HttpServer http_server(ioc, "0.0.0.0", router, port);

        http_server.setThreadNum(2).useBaseLogger().useCors().start();
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
