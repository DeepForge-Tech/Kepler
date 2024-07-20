#include <HttpServer/Middleware.hpp>

void Kepler::Middleware::add(MiddlewareFunction middleware) {
            middlewares.push_back(std::move(middleware));
        }

        void Kepler::Middleware::handle(const HttpRequest& req, HttpResponse& res, std::function<void()> next) const {
            index = 0;
            do_handle(req, res, next);
        }

        void Kepler::Middleware::do_handle(const HttpRequest& req, HttpResponse& res, std::function<void()> next) const {
            if (index < middlewares.size()) {
                middlewares[index++](req, res, [this, &req, &res, next] {
                    do_handle(req, res, next);
                });
            } else {
                next();
            }
        }