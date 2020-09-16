#pragma once
#include <string>
#include <iostream>
#include <vector>

class Requests
{
public:
	Requests();
	~Requests();

	static Requests get(std::string url, std::string params = "");
	static Requests post(std::string url, std::string data = "");

	int code = -1;
	std::string text = "";
	//std::vector<unsigned char> content;
	std::string content_type = "";

private:

    static int parseUrl(std::string url, std::string& host, std::string& port, std::string& path);

};
