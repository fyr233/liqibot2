#pragma once
#include <string>
#include <fstream>
#include <vector>

class Log
{
public:
	Log();
	~Log();

	void save();
	void add(std::string s, int64_t qq);

private:
	std::string filefolder = "data/logs/";
	std::vector<std::string> log_buf;
	int log_buf_size = 10;
};
