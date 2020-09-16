#pragma once
#include <string>

class SubProcess
{
public:
	SubProcess();
	~SubProcess();

	static std::string popen(std::string cmd);

	void subProcess();

private:

};
