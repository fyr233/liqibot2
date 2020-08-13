#include "Message.h"

using hash_t = size_t;
constexpr hash_t stringhash_prime = 0x100000001B3ull;
constexpr hash_t stringhash_basis = 0xCBF29CE484222325ull;
hash_t stringhash_run_time(const char* str, hash_t last_value = stringhash_basis) {
	return *str ? stringhash_run_time(str + 1, (*str ^ last_value) * stringhash_prime) : last_value;
}
constexpr hash_t stringhash_compile_time(const char* str, hash_t last_value = stringhash_basis) {
	return *str ? stringhash_compile_time(str + 1, (*str ^ last_value) * stringhash_prime) : last_value;
}
constexpr hash_t operator "" _hash(const char* p, size_t) {
	return stringhash_compile_time(p);
}

void Group::print()
{
}

Json::Value Group::toJson()
{
	return Json::Value();
}

Group Group::fromJson(Json::Value v)
{
	Group g;
	g.id = v["id"].asInt64();
	g.name = v["name"].asString();

	switch (stringhash_run_time(v["permission"].asString().c_str()))
	{
	case "MEMBER"_hash:
		g.permission = Group::MEMBER;
		break;

	case "ADMINISTRATOR"_hash:
		g.permission = Group::ADMINISTRATOR;
		break;

	case "OWNER"_hash:
		g.permission = Group::OWNER;
		break;

	default:
		break;
	}
	//std::cout << g.id << "\n";

	return g;
}

void Member::print()
{
}

Json::Value Member::toJson()
{
	return Json::Value();
}

Member Member::fromJson(Json::Value v)
{
	Member m;
	m.id = v["id"].asInt64();
	//std::cout << "v.isMember(\"group\")" << v.isMember("group") << "\n";
	if (v.isMember("group"))
	{
		m.memberName = v["memberName"].asString();
		m.group = Group::fromJson(v["group"]);

		switch (stringhash_run_time(v["permission"].asString().c_str()))
		{
		case "MEMBER"_hash:
			m.permission = Member::MEMBER;
			break;

		case "ADMINISTRATOR"_hash:
			m.permission = Member::ADMINISTRATOR;
			break;

		case "OWNER"_hash:
			m.permission = Member::OWNER;
			break;

		default:
			break;
		}
	}
	else
	{
		m.nickname = v["nickName"].asString();
		m.remark = v["remark"].asString();
	}
	
	return m;
}

void MessageChain::print()
{
	std::string s;
	for (int i = 0; i < chain.size(); i++)
	{
		if (chain[i].type == AMessage::Plain)
		{
			s += chain[i].text;
		}
	}
	std::cout << s << "\n";
}

Json::Value MessageChain::toJson()
{
	Json::Value v;

	for (int i = 0; i < chain.size(); i++)
	{
		Json::Value a;
		switch (chain[i].type)
		{
		case AMessage::Source:
			a["type"] = "Source";
			a["id"] = chain[i].id;
			a["time"] = chain[i].time;
			break;

		case AMessage::Plain:
			a["type"] = "Plain";
			a["text"] = chain[i].text;
			break;

		case AMessage::Image:
			a["type"] = "Image";
			a["imageId"] = chain[i].imageId;
			a["url"] = chain[i].url;
			a["path"] = chain[i].path;
			break;

		default:
			break;
		}
		v[i] = a;
	}

	return v;
}

MessageChain MessageChain::fromJson(Json::Value v)
{
	MessageChain m;
	for (int i = 0; i < v.size(); i++)
	{
		AMessage a;
		switch (stringhash_run_time(v[i]["type"].asString().c_str()))
		{
		case "Source"_hash:
			a.type = AMessage::Source;
			a.id = v[i]["id"].asInt64();
			a.time = v[i]["time"].asInt64();
			break;

		case "Plain"_hash:
			a.type = AMessage::Plain;
			a.text = v[i]["text"].asString();
			break;

		case "Image"_hash:
			a.type = AMessage::Image;
			a.imageId = v[i]["imageId"].asString();
			a.url = v[i]["url"].asString();
			a.path = v[i]["path"].asString();
			break;

		default:
			break;
		}
		m.chain.push_back(a);
	}

	return m;
}

MessageChain MessageChain::fromString(std::string s)
{
	MessageChain m;
	AMessage a;
	a.type = AMessage::Plain;
	a.text = s;
	m.chain.push_back(a);
	return m;
}

void Message::print()
{
	std::cout << toJson() << "\n";
}

Json::Value Message::toJson()
{
	return Json::Value();
}

Message Message::fromJson(Json::Value v)
{
	Message m;
	switch (stringhash_run_time(v["type"].asString().c_str()))
	{
	case "GroupMessage"_hash:
		m.type = Message::GroupMessage;
		break;

	case "FriendMessage"_hash:
		m.type = Message::FriendMessage;
		break;

	default:
		break;
	}

	m.member = Member::fromJson(v["sender"]);
	m.msgChain = MessageChain::fromJson(v["messageChain"]);

	return m;
}


