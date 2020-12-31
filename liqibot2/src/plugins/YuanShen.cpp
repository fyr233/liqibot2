#include "YuanShen.h"

#include "../SubProcess.h"

#include <regex>

YuanShen::YuanShen()
{
}

YuanShen::YuanShen(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "YuanShen";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = u8R""(
		{
			"active": true,
			"regex": ".*?打什么."
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

YuanShen::~YuanShen()
{
}

float YuanShen::metric(Message msg)
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

			if (std::regex_match(s, std::regex(config["regex"].asString())))
			{
				return 1.0;
			}

		}
	}
	return 0.0f;
}

void YuanShen::run(Message msg, QQApi* qqApi_ptr)
{
	std::string s = msg.msgChain.toString();
	s = utf8_to_ansi(s);

	//调用LianWu.py，传入s
	std::string ans = SubProcess::popen("python data/plugins/YuanShen/LianWu.py \"" + s + "\"");
	ans = ansi_to_utf8(ans);

	//发送消息
	qqApi_ptr->sendMessage(msg.member, 0, ans);
	std::cout << "YuanShen: " << ans << "\n";
}

void YuanShen::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
}

void YuanShen::onClose()
{
}
