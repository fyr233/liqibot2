#include "Default.h"

#include "cmdline.h"

Default::Default()
{
}

Default::Default(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "Default";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = u8R""(
		{
			"active": true,
			"Repeat-config": {
				"prob": 0.5,
				},
			"RandomReply-config": {
				"prob": 0.5,
				}
		}
		)"";

	config = parseJson(conf);
	loadConfig();	
}

Default::~Default()
{
}

float Default::metric(Message msg)
{
	if (config["active"].asBool())
	{
		switch (msg.type)
		{
		case Message::FriendMessage:
		case Message::GroupMessage:
			return 0.501;
			break;
		default:
			break;
		}
	}
	return 0.0;
}

void Default::run(Message msg, QQApi* qqApi_ptr)
{
	if (repeat_ptr == nullptr || randomReply_ptr == nullptr)
	{
		for (int i = 0; i < (*rt_tb_st_ptr).size(); i++)
		{
			if ((*rt_tb_st_ptr)[i]->name == "Repeat")
			{
				repeat_ptr = (*rt_tb_st_ptr)[i];
			}
			else if ((*rt_tb_st_ptr)[i]->name == "RandomReply")
			{
				randomReply_ptr = (*rt_tb_st_ptr)[i];
			}
			else if ((*rt_tb_st_ptr)[i]->name == "Setu")
			{
				setu_ptr = (*rt_tb_st_ptr)[i];
			}
		}
	}

	if (dist(gen) / (float)rand_max < config["Repeat-config"]["prob"].asFloat())
	{
		repeat_ptr->run(msg, qqApi_ptr);
	}

	if (dist(gen) / (float)rand_max < config["RandomReply-config"]["prob"].asFloat())
	{
		randomReply_ptr->run(msg, qqApi_ptr);
	}

	setu_ptr->run(msg, qqApi_ptr);
}

void Default::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: plugins/Default -c set -m Repeat -p 0.4
				 plugins/Default -c set --module RandomRepeat -p 0.6
				 plugins/Default -c get
				 plugins/Default -c get --json
	*/

	cmdline::parser p;

	p.add<int>("active", 'a', "global repeat active, 1 for enable, 0 for disable", false, 1);
	p.add<std::string>("cmd", 'c', "set or get", true, "", cmdline::oneof<std::string>("set", "get"));
	p.add("json", 'j', "raw json config, only work with \"-c get\"");
	p.add<std::string>("module", 'm', "module name", false, "");
	p.add<float>("prob", 'p', "probability", false, -1);

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
	case "set"_hash:
		if (permission_ptr->getRole(0, msg.member.id) <= Permission::VIP)
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				u8"操作失败：权限不足"
			);
			break;
		}
		if (!p.get<std::string>("module").empty() && p.get<float>("prob") >= 0)
		{
			if (config.isMember(p.get<std::string>("module") + "-config"))
			{
				config[p.get<std::string>("module") + "-config"]["prob"] = p.get<float>("prob");
			}
			else
			{
				qqApi_ptr->sendMessage(msg.member, 0, "config: unknown module: " + p.get<std::string>("module"));
				break;
			}
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

	default:
		break;
	}
}

void Default::onClose()
{
	saveConfig();
}
