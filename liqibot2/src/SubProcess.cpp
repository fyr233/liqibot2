#include "SubProcess.h"

SubProcess::SubProcess()
{
}

SubProcess::~SubProcess()
{
}

std::string SubProcess::popen(std::string cmd)
{
	FILE* pipe;
	char buffer[128];
	std::string result;

	pipe = _popen(cmd.c_str(), "r");
	//if (!pipe) return { "ERROR", };
	while (!feof(pipe))
	{
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	_pclose(pipe);
	return result;
}

void SubProcess::subProcess()
{
}
