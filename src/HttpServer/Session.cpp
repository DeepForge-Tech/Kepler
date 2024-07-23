#include <HttpServer/Session.hpp>

void Kepler::Session::start()
{ 
    do_read(); 
}

void Kepler::Session::do_read()
{
            auto self(shared_from_this());
            http::async_read(socket, buffer, request,
                [this, self](beast::error_code ec, std::size_t bytes_transferred) {
                    boost::ignore_unused(bytes_transferred);
                    if (!ec) {
                        HttpRequest req(request);
                        HttpResponse res = router.route(req);

                        send_response(res);
                    }
                });
        }

void Kepler::Session::send_response(const HttpResponse& res)
{
            auto self(shared_from_this());
            response.result(res.status());
            response.body() = res.body();
            response.prepare_payload();
            http::async_write(socket, response,
                [this, self](beast::error_code ec, std::size_t) {
                    socket.shutdown(tcp::socket::shutdown_send, ec);
                });
        }