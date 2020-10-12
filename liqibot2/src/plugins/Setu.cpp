#include "Setu.h"

#include <fstream>
#include <direct.h>  
#include <stdio.h>

#include "cmdline.h"
#include "../SubProcess.h"
#include "../Requests.h"

Setu::Setu()
{
}

Setu::Setu(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "Setu";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = u8R""(
		{
			"active": true,
			"SetuDir": "../../miraiOK_Release/data/MiraiApiHttp/images/Setu/",
			"triggers": [
				"色图"
				],
			"max-QPS": 20,
			"recall": true,
			"recall-delay": 90
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

Setu::~Setu()
{
}

float Setu::metric(Message msg)
{
	/*
	Setu模块有两种消息入口，
	1.遇到色图触发词，返回metric 1.0
	2.通过Default模块进入，执行色图检测
	*/

	//发色图
	//判断是否是色图触发词
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

void Setu::run(Message msg, QQApi* qqApi_ptr)
{
	//判断是否是色图触发词
	if (msg.type == Message::FriendMessage || msg.type == Message::GroupMessage)
	{
		std::string s = msg.msgChain.toString();

		for (int i = 0; i < config["triggers"].size(); i++)
		{
			std::string command = config["triggers"][i].asString();
			//std::cout << s << "\n" << command << "\n" << (s == command) << "\n";
			if (s == command)
			{
				//检测QPS
				timelist.resize(config["max-QPS"].asInt(), 0);
				time_t now = time(0);
				for (int j = 0; j < timelist.size(); j++)
				{
					if (now - timelist[j] > 2 || timelist[j] == 0)//未达到QPS
					{
						timelist[j] = now;
						break;
					}
					else if (j == timelist.size() - 1)//搜索达到终点，已达到QPS
					{
						//发送消息
						qqApi_ptr->sendMessage(msg.member, 0, u8"冲太快了啦");
						std::cout << "Setu: " << u8"冲太快了啦" << "\n";
						return;
					}
				}

				//发色图

				//调用SelectSetu.py，返回相对路径的文件名，以换行分隔
				std::string ans = SubProcess::popen("python data/plugins/Setu/SelectSetu.py");

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
				int msgid = qqApi_ptr->sendMessage(msg.member, 0, mc);
				std::cout << "Setu: " << mc.toString() << "\n";

				//撤回
				if (config["recall"].asBool())
				{
					std::this_thread::sleep_for(std::chrono::seconds(config["recall-delay"].asInt()));

					qqApi_ptr->recall(msgid);
					std::cout << "Setu: recall: " << mc.toString() << "\n";
				}

				return ;
			}
		}
	}

	//不是色图触发词，执行色图检测
	//遍历消息链
	for (int i = 0; i < msg.msgChain.chain.size(); i++)
	{
		if (msg.msgChain.chain[i].type == MessageChain::AMessage::Image)
		{
			//获取url
			std::string url = msg.msgChain.chain[i].url;
			//获取imageId
			std::string imageId = msg.msgChain.chain[i].imageId;
			//调用CheckSetu.py，传入url和imgid
			std::string ans = SubProcess::popen("python data/plugins/Setu/CheckSetu.py \"" + url + "\" " + imageId);
			if (ans.size() > 0)
			{
				qqApi_ptr->sendMessage(msg.member, 0, ansi_to_utf8(ans));
			}
		}
	}
	

}

void Setu::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: plugins/Setu -c add -t 色图！
				 plugins/ScoldMe -c add -i [图片]
				 /plugins/ScoldMe -c get
				 plugins/ScoldMe -c get --json
				 plugins/ScoldMe -c del -t 骂我！
				 plugins/ScoldMe -c del -i [图片]
	*/

	cmdline::parser p;

	p.add<int>("active", 'a', "global repeat active, 1 for enable, 0 for disable", false, 1);
	p.add<std::string>("cmd", 'c', "add, get or del", true, "", cmdline::oneof<std::string>("add", "get", "del"));
	p.add("json", 'j', "raw json config, only work with \"-c get\"");
	p.add<std::string>("trigger", 't', "trigger words", false, "");
	p.add<std::string>("image", 'i', "setu", false, "");
	p.add<int>("recall", 'r', "recall on/off", false, -1);
	p.add<int>("recall-delay", 'd', "recall delay", false, 0);
	p.add<int>("QPS", 'q', "QPS limit", false, 0);

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
		if (permission_ptr->getRole(0, msg.member.id) <= Permission::DefaultRole)
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				u8"操作失败：权限不足"
			);
			break;
		}
		if (!p.get<std::string>("trigger").empty())
		{
			config["triggers"].append(p.get<std::string>("trigger"));
		}
		if (p.get<int>("recall") != -1)
		{
			config["recall"] = (bool)p.get<int>("recall");
		}
		if (p.get<int>("recall-delay"))
		{
			config["recall-delay"] = p.get<int>("recall-delay");
		}
		if (!p.get<std::string>("image").empty())
		{
			std::string b64 = p.get<std::string>("image");
			MessageChain mc = MessageChain::fromString(b64);
			for (int i = 0; i < mc.chain.size(); i++)
			{
				if (mc.chain[i].type == MessageChain::AMessage::Image)
				{
					for (int j = 0; j < msg.msgChain.chain.size(); j++)
					{
						if (msg.msgChain.chain[j].type == MessageChain::AMessage::Image && msg.msgChain.chain[j].imageId == mc.chain[i].imageId)
						{
							//下载图片
							Requests r = Requests::get(msg.msgChain.chain[j].url);

							std::cout << msg.msgChain.chain[j].toJson() << "\n";
							std::cout << "\n" << r.text << "\n";
							std::ofstream outfile(config["SetuDir"].asString() + msg.msgChain.chain[j].imageId + "." + r.content_type.substr(6));
							outfile << r.text;
							outfile.close();

							break;
						}
					}
				}
			}
		}
		if (p.get<int>("QPS"))
		{
			config["max-QPS"] = p.get<int>("QPS");
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
		if (permission_ptr->getRole(0, msg.member.id) <= Permission::DefaultRole)
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				u8"操作失败：权限不足"
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
		if (p.get<std::string>("image").size())
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				u8"此功能未完成"
			);
		}
		saveConfig();
		break;

	default:
		break;
	}

}

void Setu::onClose()
{
}
