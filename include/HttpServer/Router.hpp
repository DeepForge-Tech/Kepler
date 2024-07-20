#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <HttpServer/HttpRequest.hpp>
#include <HttpServer/HttpResponse.hpp>
#include <HttpServer/Middleware.hpp>

using RequestHandler = std::function<void(const Kepler::HttpRequest&, Kepler::HttpResponse&)>;

namespace Kepler
{
    class Router {
    public:

        void get(const std::string& path, RequestHandler handler, std::vector<Kepler::Middleware::MiddlewareFunction> middleware_list = {});

        void post(const std::string& path, RequestHandler handler, std::vector<Kepler::Middleware::MiddlewareFunction> middleware_list = {});

        void use(Middleware::MiddlewareFunction middleware);

        HttpResponse route(const HttpRequest& req) const;

    private:
        using HandlerWithMiddleware = std::pair<RequestHandler, std::vector<Middleware::MiddlewareFunction>>;
        
        std::unordered_map<std::string, HandlerWithMiddleware> get_handlers;
        std::unordered_map<std::string, HandlerWithMiddleware> post_handlers;
        Middleware default_middleware;
        

        HandlerWithMiddleware find_handler(const HttpRequest& req) const;

        bool check_handler(const HttpRequest& req) const;
    };
}

#endif // ROUTER_HPP
