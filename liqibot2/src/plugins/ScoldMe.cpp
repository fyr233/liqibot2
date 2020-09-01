#include "ScoldMe.h"

#include "cmdline.h"

ScoldMe::ScoldMe()
{
}

ScoldMe::ScoldMe(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "ScoldMe";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = R""(
		{
			"active": true,
			"triggers": [
				
				],
			"replies": [
				
				],
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

ScoldMe::~ScoldMe()
{
}

float ScoldMe::metric(Message msg)
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
				if (s == command)
				{
					return 1.0;
				}
			}
		}
	}
	return 0.0f;
}

void ScoldMe::run(Message msg, QQApi* qqApi_ptr)
{
	std::random_device rd;   // non-deterministic generator  
	std::mt19937 gen = std::mt19937(rd());  // to seed mersenne twister.  
	std::uniform_int_distribution<> dist = std::uniform_int_distribution<>(0, config["replies"].size() - 1); // distribute results between 1 and 6 inclusive. 

	std::string s = config["replies"][dist(gen)].asString();
	std::cout << "ScoldMe: " << s << "\n";
	qqApi_ptr->sendMessage(msg.member, 0, s);
}

void ScoldMe::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: plugins/ScoldMe -c add -t 骂我！
				 plugins/ScoldMe -c add -r 混蛋！
				 /plugins/ScoldMe -c get
				 plugins/ScoldMe -c get --json
				 plugins/ScoldMe -c del -t 骂我！
				 plugins/ScoldMe -c del -r 混蛋！
	*/

	cmdline::parser p;

	p.add<int>("active", 'a', "global repeat active, 1 for enable, 0 for disable", false, 1);
	p.add<std::string>("cmd", 'c', "add, get or del", true, "", cmdline::oneof<std::string>("add", "get", "del"));
	p.add("json", 'j', "raw json config, only work with \"-c get\"");
	p.add<std::string>("trigger", 't', "trigger words", false, "");
	p.add<std::string>("reply", 'r', "reply words", false, "");

	try
	{
		p.parse_check(s);
	}
	catch (const std::string& e)
	{
		qqApi_ptr->sendMessage(msg.member, 0, e);
		return;
	}

	config["active"] = p.get<int>("active") ? true : false;

	switch (stringhash_run_time(p.get<std::string>("cmd").c_str()))
	{
	case "add"_hash:
		if (permission_ptr->getRole(0, msg.member.id) <= 1)
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				u8"操作失败：权限不足"
			);
			break;
		}
		if (! p.get<std::string>("trigger").empty())
		{
			config["triggers"].append(p.get<std::string>("trigger"));
		}
		if (! p.get<std::string>("reply").empty())
		{
			config["replies"].append(p.get<std::string>("reply"));
		}
		qqApi_ptr->sendMessage(msg.member, 0,
			"set success"
		);
		saveConfig();
		break;

	case "get"_hash:
		if (true)
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				dumpsJson(config)
			);
			break;
		}
		break;

	case "del"_hash:
		if (permission_ptr->getRole(0, msg.member.id) <= 1)
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				"操作失败：权限不足"
			);
			break;
		}
		if (p.get<std::string>("trigger").size())
		{
			Json::Value v;
			for (int i = 0; i < config["triggers"].size(); i++)
			{
				if (config["triggers"][i].asString() == p.get<std::string>("trigger"))
				{
					config["triggers"].removeIndex(i, &v);
					qqApi_ptr->sendMessage(msg.member, 0,
						"del trigger word " + v.asString() + " success"
					);
					break;
				}
			}
			if (v.empty())
			{
				qqApi_ptr->sendMessage(msg.member, 0,
					"trigger word " + p.get<std::string>("trigger") + " not found"
				);
			}
		}
		if (p.get<std::string>("reply").size())
		{
			Json::Value v;
			for (int i = 0; i < config["replies"].size(); i++)
			{
				if (config["replies"][i].asString() == p.get<std::string>("reply"))
				{
					config["replies"].removeIndex(i, &v);
					qqApi_ptr->sendMessage(msg.member, 0,
						"del reply word " + v.asString() + " success"
					);
					break;
				}
			}
			if (v.empty()) 
			{
				qqApi_ptr->sendMessage(msg.member, 0,
					"reply word " + p.get<std::string>("reply") + " not found"
				);
			}
		}
		saveConfig();
		break;

	default:
		break;
	}
}

void ScoldMe::onClose()
{
}
