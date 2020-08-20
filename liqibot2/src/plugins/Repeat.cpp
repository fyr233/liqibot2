#pragma once
#include "repeat.h"

#include "cmdline.h"

#include "../web_api/api_mirai_http.h"

Repeat::Repeat()
{
}

Repeat::Repeat(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr)
{
	//重要，模块名字
	Plugin::name = "Repeat";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;

	//默认config
	std::string conf = R""(
		{
			"active": true,
			"default": 0.05,
			"group-config": {
				"1": 1.0,
				},
			"member-config": {
				"1": 1.0,
				},
			"group-member-config": {
				"1-1": 1.0,
				}
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

Repeat::~Repeat()
{
}

float Repeat::metric(Message msg)
{
	if (config["active"].asBool())
	{
		if (msg.type == Message::FriendMessage || msg.type == Message::GroupMessage)
		{
			return 0.501;
		}
	}
	return 0.0;
}

void Repeat::run(Message msg, QQApi* qqApi_ptr)
{
	float prob = config["default"].asFloat();
	if (config["group-config"].isMember(std::to_string(msg.member.group.id)))
	{
		prob = config["group-config"][std::to_string(msg.member.group.id)].asFloat();
	}
	if (config["member-config"].isMember(std::to_string(msg.member.id)))
	{
		prob = config["member-config"][std::to_string(msg.member.id)].asFloat();
	}
	if (config["group-member-config"].isMember(std::to_string(msg.member.group.id) + "-" + std::to_string(msg.member.id)))
	{
		prob = config["group-member-config"][std::to_string(msg.member.group.id) + "-" + std::to_string(msg.member.id)].asFloat();
	}

	if (dist(gen) / (float)rand_max < prob)
	{
		std::cout << "repeat" << msg.msgChain.toString() << "\n";
		qqApi_ptr->sendMessage(msg.member, 0, msg.msgChain);
	}
	
}

void Repeat::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: plugins/Repeat -c set -q 123456 -g 987654 -p 0.1
				 plugins/Repeat -c set --qq 123456 --group 0 --prob 0.1
				 /plugins/Repeat -c get
				 plugins/Repeat -c get --json
				 plugins/Repeat -c del
	*/

	cmdline::parser p;

	// add specified type of variable.
	// 1st argument is long name
	// 2nd argument is short name (no short name if '\0' specified)
	// 3rd argument is description
	// 4th argument is mandatory (optional. default is false)
	// 5th argument is default value  (optional. it used when mandatory is false)
	//a.add<string>("host", 'h', "host name", true, "");

	// 6th argument is extra constraint.
	// Here, port number must be 1 to 65535 by cmdline::range().
	//a.add<int>("port", 'p', "port number", false, 80, cmdline::range(1, 65535));

	// cmdline::oneof() can restrict options.
	//a.add<string>("type", 't', "protocol type", false, "http", cmdline::oneof<string>("http", "https", "ssh", "ftp"));

	// Boolean flags also can be defined.
	// Call add method without a type parameter.
	//a.add("gzip", '\0', "gzip when transfer");

	p.add<int>("active", 'a', "global repeat active, 1 for enable, 0 for disable", false, 1);
	p.add<std::string>("cmd", 'c', "set, get or del", true, "", cmdline::oneof<std::string>("set", "get", "del"));
	p.add("json", 'j', "raw json config, only work with \"-c get\"");
	p.add<int64>("qq", 'q', "qq number", false, 0LL);
	p.add<int64>("group", 'g', "group number", false, 0LL);
	p.add<float>("prob", 'p', "repeat probability", false, 1.0);

	try
	{
		p.parse_check(s);
	}
	catch (const std::string& e)
	{
		qqApi_ptr->sendMessage(msg.member, 0, e);
	}

	auto reply = [](int64 group, int64 qq, float prob) -> std::string {
		std::string s;
		if (group)
		{
			s += "group:" + std::to_string(group) + "\n";
		}
		if (qq)
		{
			s += "qq:" + std::to_string(qq) + "\n";
		}
		s += "prob:" + std::to_string(prob) + "\n";
		return s;
	};

	switch (stringhash_run_time(p.get<std::string>("cmd").c_str()))
	{
	case "set"_hash:
		config["active"] = p.get<int>("active") ? true : false;
		if (p.get<int64>("qq") * p.get<int64>("group"))
		{
			config["group-member-config"][std::to_string(p.get<int64>("group")) + "-" + std::to_string(p.get<int64>("qq"))] = p.get<float>("prob");
		}
		else if (p.get<int64>("qq") + p.get<int64>("group") == 0)
		{
			if (msg.member.group.id == 0)//私聊
			{
				config["member-config"][std::to_string(msg.member.id)] = p.get<float>("prob");
			}
			else//群聊
			{
				config["group-member-config"][std::to_string(msg.member.group.id) + "-" + std::to_string(msg.member.id)] = p.get<float>("prob");
			}
		}
		else if (p.get<int64>("qq") == 0)
		{
			config["group-config"][std::to_string(p.get<int64>("group"))] = p.get<float>("prob");
		}
		else if (p.get<int64>("group") == 0)
		{
			config["member-config"][std::to_string(p.get<int64>("qq"))] = p.get<float>("prob");
		}
		qqApi_ptr->sendMessage(msg.member, 0,
			"set success"
		);
		saveConfig();
		break;

	case "get"_hash:
		if (p.exist("json"))
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				config.toStyledString()
			);
			break;
		}
		if (p.get<int64>("qq") * p.get<int64>("group"))
		{
			float prob = config["group-member-config"][std::to_string(p.get<int64>("group")) + "-" + std::to_string(p.get<int64>("qq"))].asFloat();
			qqApi_ptr->sendMessage(msg.member, 0,
				reply(p.get<int64>("group"), p.get<int64>("qq"), prob)
				);
		}
		else if (p.get<int64>("qq") + p.get<int64>("group") == 0)
		{
			if (msg.member.group.id == 0)//私聊
			{
				float prob = config["member-config"][std::to_string(msg.member.id)].asFloat();
				qqApi_ptr->sendMessage(msg.member, 0,
					reply(0, msg.member.id, prob)
				);
			}
			else//群聊
			{
				float prob = config["group-member-config"][std::to_string(msg.member.group.id) + "-" + std::to_string(msg.member.id)].asFloat();
				qqApi_ptr->sendMessage(msg.member, 0,
					reply(msg.member.group.id, msg.member.id, prob)
				);
			}
		}
		else if (p.get<int64>("qq") == 0)
		{
			float prob = config["group-config"][std::to_string(p.get<int64>("group"))].asFloat();
			qqApi_ptr->sendMessage(msg.member, 0,
				reply(p.get<int64>("group"), 0, prob)
			);
		}
		else if (p.get<int64>("group") == 0)
		{
			float prob = config["member-config"][std::to_string(p.get<int64>("qq"))].asFloat();
			qqApi_ptr->sendMessage(msg.member, 0,
				reply(0, p.get<int64>("qq"), prob)
			);
		}
		break;

	case "del"_hash:
		if (p.get<int64>("qq") * p.get<int64>("group"))
		{
			config["group-member-config"].removeMember(std::to_string(p.get<int64>("group")) + "-" + std::to_string(p.get<int64>("qq")));
		}
		else if (p.get<int64>("qq") + p.get<int64>("group") == 0)
		{
			if (msg.member.group.id == 0)//私聊
			{
				config["member-config"].removeMember(std::to_string(msg.member.id));
			}
			else//群聊
			{
				config["group-member-config"].removeMember(std::to_string(msg.member.group.id) + "-" + std::to_string(msg.member.id));
			}
		}
		else if (p.get<int64>("qq") == 0)
		{
			config["group-config"].removeMember(std::to_string(p.get<int64>("group")));
		}
		else if (p.get<int64>("group") == 0)
		{
			config["member-config"].removeMember(std::to_string(p.get<int64>("qq")));
		}
		qqApi_ptr->sendMessage(msg.member, 0,
			"del success"
		);
		saveConfig();
		break;

	default:
		break;
	}

}

void Repeat::onClose()
{
	saveConfig();
}
