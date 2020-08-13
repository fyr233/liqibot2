#pragma once
#include <string>
#include <iostream>

class Requests
{
public:
	Requests();
	~Requests();

	static Requests get(std::string url, std::string params = "");
	static Requests post(std::string url, std::string data = "");

	int code = -1;
	std::string text = "";

private:

    static int parseUrl(std::string url, std::string& host, std::string& port, std::string& path);

};
