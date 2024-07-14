#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <HttpServer/HttpRequest.hpp>
#include <HttpServer//HttpResponse.hpp>

using RequestHandler = std::function<HttpResponse(const HttpRequest&)>;

class Router {
public:
    void get(const std::string& path, RequestHandler handler) {
        get_handlers[path] = std::move(handler);
    }

    void post(const std::string& path, RequestHandler handler) {
        post_handlers[path] = std::move(handler);
    }

    HttpResponse route(const HttpRequest& req) const {
        std::unordered_map<std::string, RequestHandler>::const_iterator it;
        
        switch (req.method())
        {
            case boost::beast::http::verb::get:
                it = get_handlers.find(req.target());
                if (it != get_handlers.end()) {
                    return it->second(req);
                }
                break;
            
            case boost::beast::http::verb::post:
                it = post_handlers.find(req.target());
                if (it != post_handlers.end()) {
                    return it->second(req);
                }
                break;

            // You can add more cases here for other HTTP methods (e.g., PUT, DELETE)
        }
        return HttpResponse(boost::beast::http::status::not_found, "Not Found");
    }

private:
    std::unordered_map<std::string, RequestHandler> get_handlers;
    std::unordered_map<std::string, RequestHandler> post_handlers;
    std::unordered_map<std::string, RequestHandler> update_handlers;
    std::unordered_map<std::string, RequestHandler> delete_handlers;
};

#endif // ROUTER_HPP
