#include <HttpServer/HttpServer.hpp>
#include <HttpServer/Router.hpp>
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
        // if (argc != 2) {
        //     std::cerr << "Usage: http_server <port>\n";
        //     return 1;
        // }
        std::cout << argv[1] << std::endl;
        uint16_t port = argc >= 2 ? std::atoi(argv[1]) : 80;
        uint8_t num_threads = std::thread::hardware_concurrency() - 1;
        std::cout << "Server threads:" << std::to_string(num_threads) << std::endl;
        asio::io_context ioc;

        Kepler::Router router;
        router.get("/", handle_root);
        router.get("/json", handle_json);
        router.post("/post", handle_post);
        router.get("/hello", handle_hello);

        Kepler::HttpServer http_server(ioc, port, router);

        // Enable logger middleware
        http_server.listen().setLogger(true);

        std::vector<std::thread> v;
        v.reserve(num_threads);
        for (auto i = 0; i < num_threads; ++i)
        {
            v.emplace_back([&ioc]
                           { ioc.run(); });
        }

        for (std::thread &t : v)
        {
            t.join();
        }
        // logger.finished = true;       // Устанавливаем флаг завершения для потока
        // logger.bufferCv.notify_one(); // Пробуждаем поток для завершения
        // logThread.join();             // Ожидание завершения потока
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
