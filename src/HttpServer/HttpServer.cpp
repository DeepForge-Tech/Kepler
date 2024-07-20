#include <HttpServer/HttpServer.hpp>

Kepler::HttpServer &Kepler::HttpServer::useBaseLogger()
{
    if (!setCustomLogger && !setBaseLogger)
    {
        logger = std::make_shared<Logger::Logging>("Kepler.log", "10mb");
        logThread = std::thread(&Logger::Logging::processLogBuffer, logger.get());
        default_middlewares = std::make_shared<DefaultMiddlewares>(*logger);
        // router.use(default_middlewares->getLogMiddleware());
        router.use(std::bind(&DefaultMiddlewares::logMiddleware, default_middlewares.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        setBaseLogger = true;
    }
    else
    {
        throw std::logic_error(ERROR_USING_TWO_LOGGERS);
    }
    return *this;
}

Kepler::HttpServer &Kepler::HttpServer::useCustomLogger(Middleware::MiddlewareFunction log_middleware)
{
    if (!setCustomLogger && !setBaseLogger)
    {
        router.use(log_middleware);
        setCustomLogger = true;
    }
    else
    {
        throw std::logic_error(ERROR_USING_TWO_LOGGERS);
    }
    return *this;
}

Kepler::HttpServer &Kepler::HttpServer::listen()
{
    do_accept();
    return *this;
}