#pragma once
#include "Command.h"

#include "cmdline.h"

#include "../web_api/api_mirai_http.h"

Command::Command()
{
}

Command::Command(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr)
{
	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;

	std::string conf = R""(
		{
			"commands": [
				"config ",
				]
		}
		)"";

	config = parseJson(conf);
}

Command::~Command()
{
}

float Command::metric(Message msg)
{
	std::string s = msg.msgChain.toString();
	for (int i = 0; i < config["commands"].size(); i++)
	{
		std::string command = config["commands"][i].asString();
		if (s.substr(0, command.size()) == command)
		{
			return 1.0;
		}
	}
	return 0.0f;
}

void Command::run(Message msg, QQApi* qqApi_ptr)
{
	std::string s = msg.msgChain.toString();
	int idx = s.find(" ");
	switch (stringhash_run_time(s.substr(0, idx).c_str()))
	{
	case "config"_hash:
		Config(msg, s, qqApi_ptr);
		break;

	default:
		std::cout << "error command: " << s << "\n";
		break;
	}
}

void Command::Config(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: config plugins/repeat -c set -q 123456 -g 987654 -p 0.1
				 config plugins/repeat -c set --qq 123456 --group 0 --prob 0.1
				 config /plugins/repeat -c get
				 config plugins/repeat -c get --json
				 config plugins/repeat - c del 
	*/					

	std::string m = s.substr(7);
	int idx = m.find(" ");
	m = m.substr(0, idx);

	switch (stringhash_run_time(m.c_str()))
	{
	case "plugins/Repeat"_hash:
	case "/plugins/Repeat"_hash:
	case "Plugins/Repeat"_hash:
	case "/Plugins/Repeat"_hash:
		(*rt_tb_st_ptr)[0]->onCommand(msg, s.substr(7), qqApi_ptr);//0. Repeat
		break;

	default:
		qqApi_ptr->sendMessage(msg.member, 0, "config: unknown module");
		break;
	}
}
