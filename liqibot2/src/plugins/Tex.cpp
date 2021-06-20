﻿#include "Tex.h"

#include "../SubProcess.h"

Tex::Tex()
{
}

Tex::Tex(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "Tex";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = u8R""(
		{
			"active": true,
			"ImgDir": "data/net.mamoe.mirai-api-http/images/",
			"triggers": [
				"tex",
                "latex"
				]
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

Tex::~Tex()
{
}

float Tex::metric(Message msg)
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
				if (s.substr(0, s.find_first_of("\r\n")) == command)
				{
					return 1.0;
				}
			}
		}
	}
	return 0.0f;
}

void Tex::run(Message msg, QQApi* qqApi_ptr)
{
	std::string s = msg.msgChain.toString();
	std::string tex = s.substr(s.find_first_of("\r\n") + 1);
	tex = utf8_to_ansi(tex);
	//std::cout << tex << "\n";

	//调用Tex2Image.py，传入tex，返回png路径
	std::string ans = SubProcess::popen("python data/plugins/Tex/Tex2Image.py \"" + tex + "\"");

	//构造图片消息
	auto imgfilelist = splitString(ans, "\n");
	MessageChain mc;
	for (auto imgfile : imgfilelist)
	{
		AMessage a;
		a.type = AMessage::Image;
		a.Image_path = config["ImgDir"].asString() + imgfile;

		mc.chain.push_back(a);
	}

	//发送消息
	qqApi_ptr->sendMessage(msg.member, 0, mc);
	std::cout << "Tex: " << dumpsJson(mc.toJson(), false) << "\n";
}

void Tex::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
}

void Tex::onClose()
{
}
