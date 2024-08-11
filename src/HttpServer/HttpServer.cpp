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
        setLogger = true;
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
void Kepler::HttpServer::listen(const uint8_t numThread)
{
    std::string startInfo;
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string threadIdStr = std::move(ss.str());
    if (setLogger)
    {
        startInfo = fmt::format("Thread {} started with id {}", std::to_string(numThread), threadIdStr);
        logger_ptr->addLogToBuffer(Logger::info_label, startInfo, true);
    }
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
    std::string startInfo;
    // Preparation
    startMiddlewares();
    // Running server
    do_accept();
    if (setLogger)
    {
        startInfo = fmt::format("Server started on port: {}", std::to_string(port));
        logger_ptr->addLogToBuffer(Logger::info_label, startInfo, true);
    }
    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    for (int i = 0; i < numThreads; i++)
    {
        threads.emplace_back(&Kepler::HttpServer::listen, this, i + 1);
    }

    for (std::thread &t : threads)
    {
        if (t.joinable())
            t.join();
    }
    std::cout << "Start" << std::endl;
}

Kepler::HttpServer &Kepler::HttpServer::useCors()
{
    setCors = true;
    return *this;
}
