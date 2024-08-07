#ifndef WEBCLIENT_HPP
#define WEBCLIENT_HPP

#include <Kepler/General.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/sync_wait.hpp>

namespace Kepler
{
    class WebClient
    {
        public:
            cppcoro::task<Kepler
        protected:
            cppcoro::task<DB::DefaultReturnType> Kepler::WebClient::getUrlParts(const std::string &url)
        private:
        HttpClient http_client;
    };
}

#endif