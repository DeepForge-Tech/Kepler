#ifndef MIDDLEWARE_HPP
#define MIDDLEWARE_HPP

#include <HttpsServer/HttpsRequest.hpp>
#include <HttpsServer/HttpsResponse.hpp>

class Middleware
{
public:
    using MiddlewareFunction = std::function<void(const HttpRequest &, HttpResponse &, std::function<void()>)>;

    void add(MiddlewareFunction middleware)
    {
        middlewares.push_back(std::move(middleware));
    }

    void handle(const HttpRequest &req, HttpResponse &res, std::function<void()> next) const
    {
        index = 0; // Reset index for each request
        do_handle(req, res, next);
    }

    static void logMiddleware(const HttpRequest &req, HttpResponse &res, std::function<void()> next)
    {
        std::cout << "Request: " << req.target() << " " << req.body() << std::endl;
        std::string out;
        for (const auto &header : req.headers())
        {
            out += header.first + ": " + header.second + ", ";
        }
        std::cout << "Request headers:" << out << std::endl;
        std::cout << "Response: " << res.result_int() << " " << res.body() << std::endl;
        out = "";
        for (const auto &header : res.headers())
        {
            out += header.first + ": " + header.second + ", ";
        }
        std::cout << "Response headers:" << out << std::endl;
        next();
    }

private:
    void do_handle(const HttpRequest &req, HttpResponse &res, std::function<void()> next) const
    {
        if (index < middlewares.size())
        {
            middlewares[index++](req, res, [this, &req, &res, next]
                                 { do_handle(req, res, next); });
        }
        else
        {
            next();
        }
    }

    mutable size_t index = 0;
    std::vector<MiddlewareFunction> middlewares;
};

#endif // MIDDLEWARE_HPP
