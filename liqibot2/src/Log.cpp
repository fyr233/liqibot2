#include "Log.h"
#include <ctime>

Log::Log()
{
}

Log::~Log()
{
}

void Log::save()
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);

	std::string year = std::to_string(1900 + ltm.tm_year);
	std::string mon = 1 + ltm.tm_mon > 9 ? std::to_string(1 + ltm.tm_mon) : "0" + std::to_string(1 + ltm.tm_mon);
	std::string day = ltm.tm_mday > 9 ? std::to_string(ltm.tm_mday) : "0" + std::to_string(ltm.tm_mday);

	std::string filepath =
		filefolder + year + "-"
		+ mon + "-"
		+ day + ".log";

	std::ofstream f(filepath, std::ios_base::app);
	for (int i = 0; i < log_buf.size(); i++)
	{
		f << log_buf[i];
	}

	f.close();
	log_buf.resize(0);
}

void Log::save_immediately(std::string log)
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);

	std::string year = std::to_string(1900 + ltm.tm_year);
	std::string mon = 1 + ltm.tm_mon > 9 ? std::to_string(1 + ltm.tm_mon) : "0" + std::to_string(1 + ltm.tm_mon);
	std::string day = ltm.tm_mday > 9 ? std::to_string(ltm.tm_mday) : "0" + std::to_string(ltm.tm_mday);

	std::string filepath =
		filefolder + year + "-"
		+ mon + "-"
		+ day + ".log";

	std::ofstream f(filepath, std::ios_base::app);
	f << log;
	f.close();
}

void Log::add(std::string type, std::string s, int64_t qq)
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);

	std::string hour = ltm.tm_hour > 9 ? std::to_string(ltm.tm_hour) : "0" + std::to_string(ltm.tm_hour);
	std::string min = ltm.tm_min > 9 ? std::to_string(ltm.tm_min) : "0" + std::to_string(ltm.tm_min);
	std::string sec = ltm.tm_sec > 9 ? std::to_string(ltm.tm_sec) : "0" + std::to_string(ltm.tm_sec);

	std::string log =
		hour + ":" + min + ":" + sec
		+ "\t" + type + "\t" + std::to_string(qq) + "\t"
		+ s + "\n";

	save_immediately(log);
}

