#include "Permission.h"

#include <time.h>

#include "cmdline.h"

Permission::Permission()
{
	//默认config
	//time储存的是到期时间，0表示不限时
	std::string conf = R""(
		{
			"role-config": {
				"1325275429": {"role": 3, "time": 0}
				}
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

Permission::~Permission()
{
}

void Permission::saveConfig()
{
	std::ofstream config_file(config_path);
	config_file << dumpsJson(config);
	config_file.close();
}

void Permission::loadConfig()
{
	try
	{
		config = loadJson(config_path);
	}
	catch (const std::string& e)//解析失败，使用默认config
	{
		std::cout << "Load " + config_path + " failed.\n"
			<< e << "\n"
			<< "Using default config" << "\n";
	}
}

void Permission::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: permission -c set -q 123456 -g 987654 -r 1
				 permission -c set --qq 123456 --group 0 --role 2 --time 3600
				 Permission -c get -q 123456
				 Permission -c get --json
	*/

	cmdline::parser p;

	p.add<std::string>("cmd", 'c', "set or get", true, "", cmdline::oneof<std::string>("set", "get"));
	p.add("json", 'j', "raw json config, only work with \"-c get\"");
	p.add<int64>("qq", 'q', "qq number", false, 0LL);
	//p.add<int64>("group", 'g', "group number", false, 0LL);
	p.add<std::string>("role", 'r', "role of the user", false, "DefaultRole", cmdline::oneof<std::string>("DefaultRole", "VIP", "SVIP", "Admin"));
	p.add<int64>("time", 't', "time in seconds", false, 0LL);

	try
	{
		p.parse_check(s);
	}
	catch (const std::string& e)
	{
		qqApi_ptr->sendMessage(msg.member, 0, e);
		return;
	}

	auto reply = [](int64 group, int64 qq, Role role, int64 time) -> std::string {
		std::string s;
		if (group)
		{
			s += "group:" + std::to_string(group) + "\n";
		}
		if (qq)
		{
			s += "qq:" + std::to_string(qq) + "\n";
		}
		switch (role)
		{
		case Permission::DefaultRole:
			s += "role: DefaultRole\n";
			break;
		case Permission::VIP:
			s += "role: VIP\n";
			break;
		case Permission::SVIP:
			s += "role: SVIP\n";
			break;
		case Permission::Admin:
			s += "role: Admin\n";
			break;
		default:
			break;
		}
		if (time)
		{
			s += std::to_string(time) + "s left";
		}
		return s;
	};

	switch (stringhash_run_time(p.get<std::string>("cmd").c_str()))
	{
	case "set"_hash:
	{
		if (getRole(0, msg.member.id) < 3)
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				u8"操作失败：权限不足"
			);
			break;
		}
		int64 qq = p.get<int64>("qq") ? p.get<int64>("qq") : msg.member.id;
		switch (stringhash_run_time(p.get<std::string>("role").c_str()))
		{
		case "DefaultRole"_hash:
			setRole(0, qq, DefaultRole, p.get<int64>("time"));
			break;
		case "VIP"_hash:
			setRole(0, qq, VIP, p.get<int64>("time"));
			break;
		case "SVIP"_hash:
			setRole(0, qq, SVIP, p.get<int64>("time"));
			break;
		case "Admin"_hash:
			setRole(0, qq, Admin, p.get<int64>("time"));
			break;
		default:
			break;
		}
		qqApi_ptr->sendMessage(msg.member, 0,
			"set success"
		);
		saveConfig();
		break;
	}

	case "get"_hash:
	{
		if (p.exist("json"))
		{
			qqApi_ptr->sendMessage(msg.member, 0,
				dumpsJson(config)
			);
			break;
		}
		int64 qq = p.get<int64>("qq") ? p.get<int64>("qq") : msg.member.id;
		Role r = getRole(0, qq);
		qqApi_ptr->sendMessage(msg.member, 0,
			reply(0, qq, r, getTimeLeft(0, qq))
		);
		break;
	}
	default:
		break;
	}

}

Permission::Role Permission::getRole(int64 group, int64 qq)
{
	if (! config["role-config"].isMember(std::to_string(qq)))
	{
		return DefaultRole;
	}
	Json::Value v = config["role-config"][std::to_string(qq)];
	if (v["time"].asInt64() > 0)//有限时
	{
		time_t now = time(NULL);
		if (v["time"].asInt64() >= now)//未超时
		{
			return (Role)v["role"].asInt();
		}
		else//超时
		{
			config["role-config"].removeMember(std::to_string(qq));
			return DefaultRole;
		}
	}
	else//无限时
	{
		return (Role)v["role"].asInt();
	}
}

int64 Permission::getTimeLeft(int64 group, int64 qq)
{
	if (!config["role-config"].isMember(std::to_string(qq)))
	{
		return 0;
	}
	Json::Value v = config["role-config"][std::to_string(qq)];
	if (v["time"].asInt64() > 0)//有限时
	{
		time_t now = time(NULL);
		if (v["time"].asInt64() >= now)//未超时
		{
			return v["time"].asInt64();
		}
		else//超时
		{
			config["role-config"].removeMember(std::to_string(qq));
			return 0;
		}
	}
	else//无限时
	{
		return v["time"].asInt64();
	}
}

void Permission::setRole(int64 group, int64 qq, Role role)
{
	setRole(group, qq, role, 0);
}

void Permission::setRole(int64 group, int64 qq, Role role, int64 seconds)
{
	if (role == DefaultRole)
	{
		if (config["role-config"].isMember(std::to_string(qq)))
		{
			//设为DefaultRole相当于删掉
			config["role-config"].removeMember(std::to_string(qq));
		}
	}
	else
	{
		config["role-config"][std::to_string(qq)]["role"] = (int)role;
		if (seconds)
		{
			config["role-config"][std::to_string(qq)]["time"] = time(NULL) + seconds;
		}
		else
		{
			config["role-config"][std::to_string(qq)]["time"] = 0;
		}
	}
	
}
