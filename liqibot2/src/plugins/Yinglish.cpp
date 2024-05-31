#include "Yinglish.h"

#include "../SubProcess.h"

Yinglish::Yinglish()
{
}

Yinglish::Yinglish(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "Yinglish";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = u8R""(
		{
			"active": true,
			"triggers": [
				"说",
				]
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

Yinglish::~Yinglish()
{
}

float Yinglish::metric(Message msg)
{
	//判断开头是否是触发词
	if (config["active"].asBool())
	{
		if (msg.type == Message::FriendMessage || msg.type == Message::GroupMessage)
		{
			std::string s = msg.msgChain.toString();

			for (int i = 0; i < config["triggers"].size(); i++)
			{
				std::string command = config["triggers"][i].asString();
				//std::cout << s << "\n" << command << "\n" << (s == command) << "\n";
				if (s.substr(0, command.size()) == command)
				{
					return 1.0;
				}
			}
		}
	}
	return 0.0f;
}

void Yinglish::run(Message msg, QQApi* qqApi_ptr)
{
	std::string s = msg.msgChain.toString();
	for (int i = 0; i < config["triggers"].size(); i++)
	{
		std::string command = config["triggers"][i].asString();
		//std::cout << s << "\n" << command << "\n" << (s == command) << "\n";
		if (s.substr(0, command.size()) == command)
		{
			s = s.substr(command.size());
			break;
		}
	}

	s = utf8_to_ansi(s);
	//std::cout << tex << "\n";

	//调用Yinglish.py，传入s，返回转换的字符串
	std::string ans = SubProcess::popen("python data/plugins/Yinglish/Yinglish.py \"" + s + "\"");
	ans = ansi_to_utf8(ans);

	//发送消息
	qqApi_ptr->sendMessage(msg.member, 0, ans);
	std::cout << "Yinglish: " << ans << "\n";
}

void Yinglish::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: plugins/Yinglish -c add -t 说
				 /plugins/ScoldMe -c get
				 plugins/ScoldMe -c get --json
				 plugins/ScoldMe -c del -t 说
	*/


}

void Yinglish::onClose()
{
}
