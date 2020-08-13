#pragma once
#include "WebSocket.h"
#include <iostream>

WebSocket::WebSocket(
	std::string url, 
	std::function<void(void)> onHandshake,
	std::function<void(std::string)> onMessage,
	std::function<void(std::string)> onErr,
	std::function<void(void)> onClose)
{
	this->onHandshake = onHandshake;
	this->onMessage = onMessage;
	this->onErr = onErr;
	this->onClose = onClose;
	
	parseUrl(url, host, port, path);

}

WebSocket::~WebSocket()
{
}

void WebSocket::start()
{
	// Look up the domain name
	resolver_.async_resolve(
		host.c_str(),
		port.c_str(),
		beast::bind_front_handler(
			&WebSocket::on_resolve,
			shared_from_this())
	);
}

void WebSocket::run()
{
	// Run the I/O service. The call will return when
	// the socket is closed.
	ioc.run();
}

void WebSocket::on_resolve(beast::error_code ec, tcp::resolver::results_type results)
{
	if (ec)
		return fail(ec, "resolve");

	// Set the timeout for the operation
	beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

	// Make the connection on the IP address we get from a lookup
	beast::get_lowest_layer(ws_).async_connect(
		results,
		std::bind(&WebSocket::on_connect, shared_from_this(), 
			std::placeholders::_1, std::placeholders::_2
			)
	);
}

void WebSocket::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep)
{
	if (ec)
		return fail(ec, "connect");

	// Turn off the timeout on the tcp_stream, because
	// the websocket stream has its own timeout system.
	beast::get_lowest_layer(ws_).expires_never();

	// Set suggested timeout settings for the websocket
	ws_.set_option(
		websocket::stream_base::timeout::suggested(
			beast::role_type::client));

	// Set a decorator to change the User-Agent of the handshake
	ws_.set_option(websocket::stream_base::decorator(
		[](websocket::request_type& req)
		{
			req.set(http::field::user_agent,
				std::string(BOOST_BEAST_VERSION_STRING) +
				" websocket-client-async");
		}));

	// Update the host_ string. This will provide the value of the
	// Host HTTP header during the WebSocket handshake.
	// See https://tools.ietf.org/html/rfc7230#section-5.4
	host += ':' + std::to_string(ep.port());

	// Perform the websocket handshake
	ws_.async_handshake(host.c_str(), path.c_str(),
		beast::bind_front_handler(
			&WebSocket::on_handshake,
			shared_from_this()));
}

void WebSocket::on_handshake(beast::error_code ec)
{
	if (ec)
		return fail(ec, "handshake");

	(this->onHandshake)();

	// Read a message into our buffer
	ws_.async_read(
		buffer_,
		beast::bind_front_handler(
			&WebSocket::on_read,
			shared_from_this()));

	/*
	// Send the message
	ws_.async_write(
		net::buffer(text),
		beast::bind_front_handler(
			&WebSocket::on_write,
			shared_from_this()));
	*/
}

void WebSocket::on_write(beast::error_code ec, std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);

	if (ec)
		return fail(ec, "write");

	// Read a message into our buffer
	ws_.async_read(
		buffer_,
		beast::bind_front_handler(
			&WebSocket::on_read,
			shared_from_this()));
}

void WebSocket::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);

	if (ec)
		return fail(ec, "read");

	std::string res = beast::buffers_to_string(buffer_.data());
	buffer_.clear();
	if (res.size() == 0)
	{
		// Close the WebSocket connection
		ws_.async_close(websocket::close_code::normal,
			beast::bind_front_handler(
				&WebSocket::on_close,
				shared_from_this()));
		return;
	}
	(this->onMessage)(res);

	// Read a message into our buffer
	ws_.async_read(
		buffer_,
		beast::bind_front_handler(
			&WebSocket::on_read,
			shared_from_this()));

}

void WebSocket::on_close(beast::error_code ec)
{
	if (ec)
		return fail(ec, "close");

	// If we get here then the connection is closed gracefully

	// The make_printable() function helps print a ConstBufferSequence
	std::cout << beast::make_printable(buffer_.data()) << std::endl;

	(this->onClose)();
}

int WebSocket::parseUrl(std::string url, std::string& host, std::string& port, std::string& path)
{
	const std::string& ws___ = "ws://";
	const std::string& wss___ = "wss://";

	// 截断http协议头
	if (url.find(ws___) == 0) {
		url = url.substr(ws___.length());
	}
	else if (url.find(wss___) == 0) {
		url = url.substr(wss___.length());
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

// Report a failure
void WebSocket::fail(beast::error_code ec, char const* what)
{
	std::cerr << what << ": " << ec.message() << "\n";
	(this->onErr)(ec.message());
}
