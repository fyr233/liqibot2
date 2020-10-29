#include "Statistics.h"

#include "cmdline.h"
#include "../SubProcess.h"

Statistics::Statistics()
{
}

Statistics::Statistics(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "Statistics";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = u8R""(
		{
			"active": true,
			"triggers": [
				"统计"
				],
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

Statistics::~Statistics()
{
}

float Statistics::metric(Message msg)
{
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

void Statistics::run(Message msg, QQApi* qqApi_ptr)
{
	std::string s = msg.msgChain.toString();		// "统计 20201027"
	int spaceindex = s.find(" ");
	std::string parameters;
	if (spaceindex > 0 && spaceindex < s.size())
	{
		parameters = s.substr(s.find_first_of(" "));	// " 20201027"
	}
	parameters = utf8_to_ansi(parameters);

	//调用Statistics.py，传入parameters，返回png路径
	std::string ans = SubProcess::popen("python data/plugins/Statistics/Statistics.py " + parameters);

	//构造图片消息
	auto imgfilelist = splitString(ans, "\n");
	MessageChain mc;
	for (auto imgfile : imgfilelist)
	{
		MessageChain::AMessage a;
		a.type = MessageChain::AMessage::Image;
		a.path = imgfile;

		mc.chain.push_back(a);
	}

	//发送消息
	qqApi_ptr->sendMessage(msg.member, 0, mc);
	std::cout << "Statistics: " << dumpsJson(mc.toJson(), false) << "\n";

}

void Statistics::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
}

void Statistics::onClose()
{
}
