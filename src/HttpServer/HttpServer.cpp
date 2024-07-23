#include <HttpServer/HttpServer.hpp>

Kepler::HttpServer &Kepler::HttpServer::useBaseLogger()
{
    setBaseLogger = true;
    return *this;
}

Kepler::HttpServer &Kepler::HttpServer::useCustomLogger(Middleware::MiddlewareFunction log_middleware)
{
    router.use(log_middleware);
    setCustomLogger = true;
    return *this;
}

Kepler::HttpServer& Kepler::HttpServer::setLogPath(const std::string path)
{
    logPath = path;
    return *this;
}

Kepler::HttpServer& Kepler::HttpServer::setLogSize(const std::string size)
{
    logSize = size;
    return *this;
}

Kepler::HttpServer& Kepler::HttpServer::setThreadNum(uint8_t num_threads)
{
    numThreads = std::move(num_threads);
    if (numThreads == maxThreads)
    {
        numThreads--;
    }
    return *this;
}

Kepler::HttpServer &Kepler::HttpServer::multithread()
{
    numThreads = maxThreads - 1;
    return *this;
}
void Kepler::HttpServer::listen()
{
    ioc.run();
}

void Kepler::HttpServer::start()
{
    // Preparation
    if(setBaseLogger && setCustomLogger)
    {
        throw std::logic_error(ERROR_USING_TWO_LOGGERS);
    }
    if (setBaseLogger)
    {
        logger = std::make_shared<Logger::Logging>(logPath.c_str(), logSize.c_str());
        logThread = std::thread(&Logger::Logging::processFormattedLogBuffer, logger.get());
        default_middlewares = std::make_shared<DefaultMiddlewares>(*logger);
        router.use(std::bind(&DefaultMiddlewares::logMiddleware, default_middlewares.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
    // Running server
    do_accept();
    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    for (int i = 0; i < numThreads; i++)
    {
        threads.emplace_back(&Kepler::HttpServer::listen,this);
    }

    for (std::thread &t : threads)
    {
        if (t.joinable()) t.join();
    }
}

Kepler::HttpServer &Kepler::HttpServer::useCors()
{
    router.use(std::bind(&DefaultMiddlewares::corsMiddleware, default_middlewares.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    return *this;
}