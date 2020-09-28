#pragma once
#include "Command.h"

#include "cmdline.h"

#include "../web_api/api_mirai_http.h"

Command::Command()
{
}

Command::Command(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "Command";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = R""(
		{
			"commands": [
				"config ",
				"permission ",
				"Permission "
				]
		}
		)"";

	config = parseJson(conf);
	loadConfig();
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

	case "permission"_hash:
	case "Permission"_hash:
		permission_ptr->onCommand(msg, s, qqApi_ptr);
		break;

	default:
		std::cout << "error command: " << s << "\n";
		break;
	}
}

void Command::onClose()
{
	saveConfig();
}

void Command::Config(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: config plugins/Repeat -c set -q 123456 -g 987654 -p 0.1
				 config plugins/Repeat -c set --qq 123456 --group 0 --prob 0.1
				 config plugins/Repeat -c get
				 config plugins/Repeat -c get --json
				 config plugins/Repeat -c del 
				 config reload plugins/Repeat
	*/					
	//已放弃对 /plugins/.. 的兼容

	std::vector<std::string> words = splitString(s, " ");
	if (words.size() < 3)
	{
		qqApi_ptr->sendMessage(msg.member, 0, u8"config: 不完整的指令");
		return;
	}

	std::string word1p1 = splitString(words[1], "/")[0];	// m = plugins 或 reload，有可能为空
	switch (stringhash_run_time(word1p1.c_str()))
	{
	case "plugins"_hash:
	case "Plugins"_hash:
	{
		std::string mdl = splitString(words[1], "/")[1];	// n = Repeat，若越界则崩溃
		for (int i = 0; i < (*rt_tb_st_ptr).size(); i++)
		{
			if ((*rt_tb_st_ptr)[i]->name == mdl)
			{
				(*rt_tb_st_ptr)[i]->onCommand(msg, s.substr(7), qqApi_ptr);
			}
		}
		break;
	}
	case "reload"_hash:
	{
		std::string mdl = splitString(words[2], "/")[1];	// n = Repeat，若越界则崩溃
		for (int i = 0; i < (*rt_tb_st_ptr).size(); i++)
		{
			if ((*rt_tb_st_ptr)[i]->name == mdl)
			{
				(*rt_tb_st_ptr)[i]->loadConfig();
				qqApi_ptr->sendMessage(msg.member, 0, u8"config reload " + (*rt_tb_st_ptr)[i]->name + " success");
			}
		}
		break;
	}

	default:
		qqApi_ptr->sendMessage(msg.member, 0, u8"config: 未知的指令");
		break;
	}
}
