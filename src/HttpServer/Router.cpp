#include <HttpServer/Router.hpp>

void Kepler::Router::get(const std::string &path, RequestHandler handler, std::vector<Kepler::Middleware::MiddlewareFunction> middleware_list)
{
    get_handlers[path] = {std::move(handler), std::move(middleware_list)};
}

void Kepler::Router::post(const std::string &path, RequestHandler handler, std::vector<Kepler::Middleware::MiddlewareFunction> middleware_list)
{
    post_handlers[path] = {std::move(handler), std::move(middleware_list)};
}

void Kepler::Router::use(Middleware::MiddlewareFunction middleware)
{
    default_middleware.add(std::move(middleware));
}

Kepler::HttpResponse Kepler::Router::route(const Kepler::HttpRequest &req) const
{
    HttpResponse res(boost::beast::http::status::ok, "");
    bool handler_exist = this->check_handler(req);
    if (!handler_exist)
    {
        res = HttpResponse(boost::beast::http::status::not_found, "404 Not Found");
    }
    auto [handler, middleware_list] = find_handler(req);

    Middleware combined_middleware = default_middleware;
    for (auto &middleware : middleware_list)
    {
        combined_middleware.add(std::move(middleware));
    }

    combined_middleware.handle(req, res, [&]
                               {
                if (handler) {
                    handler(req, res);
                } });

    // Copy request headers to response
    for (const auto &header : req.headers())
    {
        res.set_header(header.first, header.second);
    }

    return res;
}

Kepler::Router::HandlerWithMiddleware Kepler::Router::find_handler(const HttpRequest &req) const
{
    std::unordered_map<std::string, HandlerWithMiddleware>::const_iterator it;
    switch (req.method())
    {
    case boost::beast::http::verb::get:
        it = get_handlers.find(req.target());
        if (it != get_handlers.end())
        {
            return it->second;
        }
        break;
    case boost::beast::http::verb::post:
        it = post_handlers.find(req.target());
        if (it != post_handlers.end())
        {
            return it->second;
        }
        break;
    // Add cases for other HTTP methods as needed
    default:
        break;
    }
    return {nullptr, {}};
}

bool Kepler::Router::check_handler(const HttpRequest &req) const
{
    switch (req.method())
    {
    case boost::beast::http::verb::get:
        return get_handlers.find(req.target()) != get_handlers.end();
        // break;
    case boost::beast::http::verb::post:
        return post_handlers.find(req.target()) != post_handlers.end();
        // break;
    // Add cases for other HTTP methods as needed
    default:
        return false;
        // break;
    }
}