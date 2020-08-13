#include "requests.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/lexical_cast.hpp>



namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

Requests::Requests()
{
}

Requests::~Requests()
{
}

Requests Requests::get(std::string url, std::string params)
{
	std::string host;
	std::string port;
	std::string path;
	parseUrl(url, host, port, path);

	Requests r = Requests();

	try
	{
		// The io_context is required for all I/O
		net::io_context ioc;

		// These objects perform our I/O
		tcp::resolver resolver(ioc);
		beast::tcp_stream stream(ioc);

		// Look up the domain name
		auto const results = resolver.resolve(host, port);

		// Make the connection on the IP address we get from a lookup
		stream.connect(results);

		// Set up an HTTP GET request message
		http::request<http::string_body> req{ http::verb::get, path, 11 };
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		req.set(http::field::connection, "keep-alive");

		// Send the HTTP request to the remote host
		http::write(stream, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(stream, buffer, res);

		r.code = (int)res.result();
		r.text = beast::buffers_to_string(res.body().data());

		// Gracefully close the socket
		beast::error_code ec;
		stream.socket().shutdown(tcp::socket::shutdown_both, ec);

		// not_connected happens sometimes
			// so don't bother reporting it.
			//
		if (ec && ec != beast::errc::not_connected)
			throw beast::system_error{ ec };

		// If we get here then the connection is closed gracefully
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		r.code = -1;
		return r;
	}

	return r;
}

Requests Requests::post(std::string url, std::string data)
{
	std::string host;
	std::string port;
	std::string path;
	parseUrl(url, host, port, path);

	Requests r = Requests();

	try
	{
		// The io_context is required for all I/O
		net::io_context ioc;

		// These objects perform our I/O
		tcp::resolver resolver(ioc);
		beast::tcp_stream stream(ioc);

		// Look up the domain name
		auto const results = resolver.resolve(host, port);

		// Make the connection on the IP address we get from a lookup
		stream.connect(results);

		// Set up an HTTP POST request message
		http::request<http::string_body> req{ http::verb::post, path, 10 };
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		req.set(http::field::content_type, "application/json; charset=UTF-8");
		req.set(http::field::connection, "keep-alive");
		req.body() = data;
		req.prepare_payload();
		//std::cout << req << "\n";

		// Send the HTTP request to the remote host
		http::write(stream, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(stream, buffer, res);

		r.code = (int)res.result();
		r.text = beast::buffers_to_string(res.body().data());

		// Gracefully close the socket
		beast::error_code ec;
		stream.socket().shutdown(tcp::socket::shutdown_both, ec);

		// not_connected happens sometimes
			// so don't bother reporting it.
			//
		if (ec && ec != beast::errc::not_connected)
			throw beast::system_error{ ec };

		// If we get here then the connection is closed gracefully
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		r.code = -1;
		return r;
	}

	return r;
}

int Requests::parseUrl(std::string url, std::string& host, std::string& port, std::string& path)
{
	const std::string& http___ = "http://";
	const std::string& https___ = "https://";

	// 截断http协议头
	if (url.find(http___) == 0) {
		url = url.substr(http___.length());
	}
	else if (url.find(https___) == 0) {
		url = url.substr(https___.length());
	}
	else {
		return -1;
	}

	// 解析域名
	std::size_t idx = url.find('/');
	// 解析 域名后的path
	if (std::string::npos == idx) {
		path = "/";
		idx = url.size();
	}
	else {
		path = url.substr(idx);
	}
	//host:port
	host = url.substr(0, idx);

	idx = host.find(':');
	if (std::string::npos == idx) {
		port = "80";
	}
	else {
		port = host.substr(idx + 1);
		host = host.substr(0, idx);
	}

	return 0;
}

