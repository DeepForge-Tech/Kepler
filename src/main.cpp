#include <HttpServer/HttpServer.hpp>
#include <HttpServer/Router.hpp>

HttpResponse handle_root(const HttpRequest& req) {
    std::string body = "<html><body><h1>Welcome to the HTTP Server</h1></body></html>";
    return HttpResponse(boost::beast::http::status::ok, body);
}

HttpResponse handle_json(const HttpRequest& req) {
    json::value jv = {{"message", "Hello, JSON!"}};
    return HttpResponse(boost::beast::http::status::ok, json::serialize(jv));
}

HttpResponse handle_post(const HttpRequest& req) {
    json::value jv = json::parse(req.body());
    return HttpResponse(boost::beast::http::status::ok, json::serialize(jv));
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: http_server <port>\n";
            return 1;
        }

        auto const port = static_cast<unsigned short>(std::atoi(argv[1]));
        uint8_t num_threads = std::thread::hardware_concurrency();
        asio::io_context ioc{1};

        Router router;
        router.get("/", handle_root);
        router.get("/json", handle_json);
        router.post("/post", handle_post);

        HttpServer http_server(ioc, port, router);

        std::vector<std::thread> v;
        v.reserve(4);
        for (auto i = 0; i < num_threads; ++i) {
            v.emplace_back([&ioc] { ioc.run(); });
        }

        for (auto& t : v) {
            t.join();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
