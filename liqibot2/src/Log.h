#pragma once
#include <string>
#include <fstream>
#include <vector>

class Log
{
public:
	Log();
	~Log();

	static void save();
	static void add_recv(std::string s, int64_t qq);
	static void add_send(std::string s, int64_t qq);

private:
	static inline std::string filefolder = "data/logs/";
	static inline std::vector<std::string> log_buf;
	static inline int log_buf_size = 10;
};
