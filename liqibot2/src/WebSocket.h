#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <functional>
#include <string>
#include <share.h>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class WebSocket : public std::enable_shared_from_this<WebSocket>
{
public:
	explicit
	WebSocket(
		std::string url, 
		std::function<void(void)> onHandshake,
		std::function<void(std::string)> onMessage,
		std::function<void(std::string)> onErr,
		std::function<void(void)> onClose
		);

	~WebSocket();

	void start();
	void run();

	std::function<void(void)> onHandshake;
	std::function<void(std::string)> onMessage;
	std::function<void(std::string)> onErr;
	std::function<void(void)> onClose;

private:

	// The io_context is required for all I/O
	net::io_context ioc;
	tcp::resolver resolver_ = tcp::resolver(net::make_strand(ioc));
	websocket::stream<beast::tcp_stream> ws_ = websocket::stream<beast::tcp_stream>(net::make_strand(ioc));
	beast::flat_buffer buffer_;
	std::string host;
	std::string port;
	std::string path;
	std::string text = "Hello";

	void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
	void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);
	void on_handshake(beast::error_code ec);
	void on_write(beast::error_code ec, std::size_t bytes_transferred);
	void on_read(beast::error_code ec, std::size_t bytes_transferred);
	void on_close(beast::error_code ec);

	static int parseUrl(std::string url, std::string& host, std::string& port, std::string& path);
	void fail(beast::error_code ec, char const* what);

};
