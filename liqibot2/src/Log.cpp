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

	std::string filepath =
		filefolder + std::to_string(1900 + ltm.tm_year) + "-"
		+ std::to_string(1 + ltm.tm_mon) + "-"
		+ std::to_string(ltm.tm_mday) + ".log";

	std::ofstream f(filepath, std::ios_base::app);
	for (int i = 0; i < log_buf.size(); i++)
	{
		f << log_buf[i];
	}

	f.close();
	log_buf.clear();
}

void Log::add(std::string s, int64_t qq)
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);

	std::string log =
		std::to_string(ltm.tm_hour) + ":" + std::to_string(ltm.tm_min) + ":" + std::to_string(ltm.tm_sec)
		+ "\t" + std::to_string(qq) + "\t" + s + "\n";

	log_buf.push_back(log);

	if (log_buf.size() >= log_buf_size)
	{
		save();
	}
}
