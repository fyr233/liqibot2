#include "Coding.h"

#include <fstream>

#include "../SubProcess.h"

Coding::Coding()
{
}

Coding::Coding(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "Coding";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = u8R""(
		{
			"active": true,
			"triggers": [
				"c",
                "cpp",
				"python"
				]
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

Coding::~Coding()
{
}

float Coding::metric(Message msg)
{
	//判断是否是触发词
	if (config["active"].asBool())
	{
		if (msg.type == Message::FriendMessage || msg.type == Message::GroupMessage)
		{
			std::string s = msg.msgChain.toString();
			if (s.size() < 1)
			{
				return 0.0f;
			}

			for (int i = 0; i < config["triggers"].size(); i++)
			{
				std::string command = config["triggers"][i].asString();
				//std::cout << s << "\n" << command << "\n" << (s == command) << "\n";
				if (s.substr(0, s.find_first_of("\r\n")) == command || 
					s.substr(0, s.find_first_of("\r\n")).substr(0, ("run" + command).size()) == "run" + command)
				{
					return 1.0;
				}
			}
		}
	}
	return 0.0f;
}

void Coding::run(Message msg, QQApi* qqApi_ptr)
{
	std::string s = msg.msgChain.toString();

	if (s.substr(0, 3)=="run")//运行代码
	{
		std::string type = s.substr(0, s.find_first_of(" ")).substr(3);
		std::string params = s.substr(s.find_first_of(" ") + 1);
		type = utf8_to_ansi(type);
		params = utf8_to_ansi(params);

		//调用RunCode.py，传入type和文件名，返回执行结果
		std::string ans = SubProcess::popen("python data/plugins/Coding/RunCode.py " + type + " " + std::to_string(msg.member.id) + "." + type + " " + params);

		ans = ansi_to_utf8(ans);
		MessageChain mc = MessageChain::fromString(ans);

		//发送消息
		qqApi_ptr->sendMessage(msg.member, 0, mc);
		std::cout << "Coding: " << dumpsJson(mc.toJson(), false) << "\n";
	}
	else//保存代码
	{
		std::string type = s.substr(0, s.find_first_of("\r\n"));
		type = utf8_to_ansi(type);
		std::string code = s.substr(s.find_first_of("\r\n") + 1);
		std::ofstream outfile("data/plugins/Coding/docker/codes/" + std::to_string(msg.member.id) + "." + type);
		outfile << code;
		outfile.close();
	}
}

void Coding::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
}

void Coding::onClose()
{
}
