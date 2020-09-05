#include "RandomReply.h"

#include "cmdline.h"

RandomReply::RandomReply()
{
}

RandomReply::RandomReply(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "RandomReply";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = u8R""(
		{
			"active": true,
			"prob": 0.05,
			"reply": [
				"草",
				],
			"group-config": {
				"1": [
					"群专有回复词",
					]
				}
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

RandomReply::~RandomReply()
{
}

float RandomReply::metric(Message msg)
{
	return 0.0f;
}

void RandomReply::run(Message msg, QQApi* qqApi_ptr)
{
	float prob = config["prob"].asFloat();
	if (dist(gen) / (float)rand_max < prob)
	{
		std::string group = std::to_string(msg.member.group.id);
		if (config["group-config"].isMember(group))//有专有回复
		{
			int id = dist(gen) % (config["reply"].size() + config["group-config"][group].size());
			if (id < config["reply"].size())
			{
				std::string r = config["reply"][id].asString();
				std::cout << "RandomReply: " << r << "\n";
				qqApi_ptr->sendMessage(msg.member, 0, r);
			}
			else
			{
				std::string r = config["group-config"][group][id - config["reply"].size()].asString();
				std::cout << "RandomReply: " << r << "\n";
				qqApi_ptr->sendMessage(msg.member, 0, r);
			}
		}
		else
		{
			int id = dist(gen) % config["reply"].size();
			std::string r = config["reply"][id].asString(); 
			std::cout << "RandomReply: " << r << "\n";
			qqApi_ptr->sendMessage(msg.member, 0, r);
		}
	}
}

void RandomReply::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: plugins/RandomReply -c add -g 987654 -r 草
				 plugins/RandomReply -c add -r 草
				 plugins/RandomReply -c get
				 plugins/RandomReply -c get --json
				 plugins/RandomReply -c del -g 987654 -r 草
				 plugins/RandomReply -c del -r 草
	*/

	cmdline::parser p;

	p.add<int>("active", 'a', "global repeat active, 1 for enable, 0 for disable", false, 1);
	p.add<std::string>("cmd", 'c', "add, get or del", true, "", cmdline::oneof<std::string>("add", "get", "del"));
	p.add("json", 'j', "raw json config, only work with \"-c get\"");
	p.add<int64>("group", 'g', "group number", false, 0LL);
	p.add<std::string>("reply", 'r', "reply words", false, "");
	p.add<float>("prob", 'p', "reply probability", false, -1.0);

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
		if (p.get<int64>("group"))
		{
			if (!p.get<std::string>("reply").empty())
			{
				config["group-config"][std::to_string(p.get<int64>("group"))].append(p.get<std::string>("reply"));
			}
		}
		else
		{
			if (permission_ptr->getRole(0, msg.member.id) <= Permission::VIP)
			{
				qqApi_ptr->sendMessage(msg.member, 0,
					u8"操作失败：权限不足"
				);
				break;
			}
			if (!p.get<std::string>("reply").empty())
			{
				config["reply"].append(p.get<std::string>("reply"));
			}
		}
		if (p.get<float>("prob") >= 0)
		{
			if (permission_ptr->getRole(0, msg.member.id) <= Permission::VIP)
			{
				qqApi_ptr->sendMessage(msg.member, 0,
					u8"操作失败：权限不足"
				);
				break;
			}
			config["prob"] = p.get<float>("prob");
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
				"操作失败：权限不足"
			);
			break;
		}
		if (p.get<int64>("group"))
		{
			Json::Value v;
			for (int i = 0; i < config["group-config"][std::to_string(p.get<int64>("group"))].size(); i++)
			{
				if (config["group-config"][std::to_string(p.get<int64>("group"))][i].asString() == p.get<std::string>("reply"))
				{
					config["group-config"][std::to_string(p.get<int64>("group"))].removeIndex(i, &v);
					qqApi_ptr->sendMessage(msg.member, 0,
						"del trigger word " + v.asString() + " success"
					);
					break;
				}
			}
			if (v.empty())
			{
				qqApi_ptr->sendMessage(msg.member, 0,
					"reply text " + p.get<std::string>("trigger") + " not found"
				);
			}
		}
		else
		{
			if (permission_ptr->getRole(0, msg.member.id) <= Permission::VIP)
			{
				qqApi_ptr->sendMessage(msg.member, 0,
					u8"操作失败：权限不足"
				);
				break;
			}

			Json::Value v;
			for (int i = 0; i < config["reply"].size(); i++)
			{
				if (config["reply"][i].asString() == p.get<std::string>("reply"))
				{
					config["reply"].removeIndex(i, &v);
					qqApi_ptr->sendMessage(msg.member, 0,
						"del reply text " + v.asString() + " success"
					);
					break;
				}
			}
			if (v.empty())
			{
				qqApi_ptr->sendMessage(msg.member, 0,
					"reply text " + p.get<std::string>("reply") + " not found"
				);
			}
		}
		saveConfig();
		break;

	default:
		break;
	}
}

void RandomReply::onClose()
{
	saveConfig();
}
