#include <HttpServer/DefaultMiddlewares.hpp>

void Kepler::DefaultMiddlewares::logMiddleware(const HttpRequest &req, HttpResponse &res, std::function<void()> next) const
        {
            // return [this](const HttpRequest &req, HttpResponse &res, std::function<void()> next)
            // {
                std::stringstream ss;
            ss << std::this_thread::get_id();
            std::string threadIdStr = std::move(ss.str());
            ss.str().resize(0); // савмый стопудовый способ
            ss.str().clear();
            ss.str("");
            ss << req.method();
            std::string method = std::move(ss.str());
            std::string logText = fmt::format("Thread: {} | {} {} {}", threadIdStr, method, req.target(), std::to_string(res.result_int()));
            logger.addLogToBuffer(logText);
            next();
            // };
        }