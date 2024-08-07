#include <Kepler/HttpServer/HttpServer.hpp>

template <typename ClassType>
Kepler::HttpServer &Kepler::HttpServer::useCustomLogger(ClassType middlewareClass)
{
    if (!setLogger)
    {
        setLogger = true;
    }
    else
    {
        throw std::runtime_error("Logger is already in use");
    }
    return *this;
}

Kepler::HttpServer &Kepler::HttpServer::useBaseLogger()
{
    if (!setLogger)
    {
        setLogger = true ;
        logger_ptr = std::make_shared<Logger::Logging>(logPath.c_str(), logSize.c_str());
        logThread = std::thread(&Logger::Logging::processFormattedLogBuffer, logger_ptr.get());
    }
    else
    {
        throw std::runtime_error("Logger is already in use");
    }

    return *this;
}

Kepler::HttpServer &Kepler::HttpServer::setLogPath(const std::string path)
{
    logPath = path;
    return *this;
}

Kepler::HttpServer &Kepler::HttpServer::setLogSize(const std::string size)
{
    logSize = size;
    return *this;
}

Kepler::HttpServer &Kepler::HttpServer::setThreadNum(uint8_t num_threads)
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
    numThreads = maxThreads--;
    return *this;
}
void Kepler::HttpServer::listen()
{
    ioc.run();
}

void Kepler::HttpServer::startMiddlewares()
{
    default_middlewares_ptr = std::make_shared<DefaultMiddlewares>(logger_ptr.get());
    if (setLogger)
    {
        router.use(std::bind(&DefaultMiddlewares::logMiddleware, default_middlewares_ptr.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
    else
    {
        default_middlewares_ptr = std::make_shared<DefaultMiddlewares>(nullptr);
    }
    if (setCors)
        router.use(std::bind(&DefaultMiddlewares::corsMiddleware, default_middlewares_ptr.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void Kepler::HttpServer::start()
{
    // Preparation
    // if (setBaseLogger && setCustomLogger)
    // {
    //     throw std::logic_error(ERROR_USING_TWO_LOGGERS);
    // }
    startMiddlewares();
    // Running server
    do_accept();
    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    for (int i = 0; i < numThreads; i++)
    {
        threads.emplace_back(&Kepler::HttpServer::listen, this);
    }

    for (std::thread &t : threads)
    {
        if (t.joinable())
            t.join();
    }
}

Kepler::HttpServer &Kepler::HttpServer::useCors()
{
    setCors = true;
    return *this;
}
