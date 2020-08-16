#pragma once
#include "Message.h"



#include "web_api/api_mirai_http.h"




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
		v[i] = chain[i].toJson();
	}

	return v;
}

MessageChain MessageChain::fromJson(Json::Value v)
{
	MessageChain m;
	for (int i = 0; i < v.size(); i++)
	{
		m.chain.push_back(AMessage::fromJson(v[i]));
	}

	return m;
}

std::string MessageChain::toString()
{
	std::string s;
	for (int i = 0; i < chain.size(); i++)
	{
		switch (chain[i].type)
		{
		case AMessage::Plain:
			s += chain[i].text;
			break;

		case AMessage::Image:
			s += "[b64:" + base64::encode(dumpsJson(chain[i].toJson())) + "]";
			break;

		default:
			break;
		}
	}

	return s;
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
	Json::Value v;
	switch (type)
	{
	case Message::FriendMessage:
		v["type"] = "FriendMessage";
		v["sender"] = member.toJson();
		v["messageChain"] = msgChain.toJson();
		break;

	case Message::GroupMessage:
		v["type"] = "FriendMessage";
		v["sender"] = member.toJson();
		v["messageChain"] = msgChain.toJson();
		break;
		
	default:
		v["type"] = "None";
		break;
	}

	return v;
}

Message Message::fromJson(Json::Value v)
{
	Message m;
	switch (stringhash_run_time(v["type"].asString().c_str()))
	{
	case "GroupMessage"_hash:
		m.type = Message::GroupMessage;
		m.member = Member::fromJson(v["sender"]);
		m.msgChain = MessageChain::fromJson(v["messageChain"]);
		break;

	case "FriendMessage"_hash:
		m.type = Message::FriendMessage;
		m.member = Member::fromJson(v["sender"]);
		m.msgChain = MessageChain::fromJson(v["messageChain"]);
		break;

	default:
		break;
	}

	return m;
}

Json::Value MessageChain::AMessage::toJson()
{
	Json::Value v;
	switch (type)
	{
	case AMessage::Source:
		v["type"] = "Source";
		v["id"] = id;
		v["time"] = time;
		break;

	case AMessage::Plain:
		v["type"] = "Plain";
		v["text"] = text;
		break;

	case AMessage::Image:
		v["type"] = "Image";
		v["imageId"] = imageId;
		v["url"] = url;
		v["path"] = path;
		break;

	default:
		break;
	}

	return v;
}

MessageChain::AMessage MessageChain::AMessage::fromJson(Json::Value v)
{
	AMessage a;
	switch (stringhash_run_time(v["type"].asString().c_str()))
	{
	case "Source"_hash:
		a.type = AMessage::Source;
		a.id = v["id"].asInt64();
		a.time = v["time"].asInt64();
		break;

	case "Plain"_hash:
		a.type = AMessage::Plain;
		a.text = v["text"].asString();
		break;

	case "Image"_hash:
		a.type = AMessage::Image;
		a.imageId = v["imageId"].asString();
		a.url = v["url"].asString();
		a.path = v["path"].asString();
		break;

	default:
		break;
	}

	return a;
}
