#pragma once
#include "Message.h"

#include <regex>

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
	Json::Value v;
	v["id"] = id;
	if (group.id != 0)
	{
		v["memberName"] = memberName;
		v["group"] = group.toJson();
		switch (permission)
		{
		case Member::MEMBER:
			v["permission"] = "MEMBER";
			break;
		case Member::ADMINISTRATOR:
			v["permission"] = "ADMINISTRATOR";
			break;
		case Member::OWNER:
			v["permission"] = "OWNER";
			break;
		default:
			break;
		}
	}
	else
	{
		v["nickName"] = nickname;
		v["remark"] = remark;
	}
	return v;
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
			s += chain[i].Plain_text;
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
			s += chain[i].Plain_text;
			break;

		case AMessage::Image:
		{
			Json::Value v = chain[i].toJson();
			v.removeMember("url");
			v.removeMember("path");
			s += "[b64:" + base64::encode(dumpsJson(v)) + "]";
		}
			break;

		default:
			break;
		}
	}

	return s;
}

MessageChain MessageChain::fromString(std::string s)
{
	MessageChain mc;

	std::regex reg("\\[b64:(.*?)\\]");
	std::smatch m;
	auto pos = s.cbegin();
	auto end = s.cend();
	for (; std::regex_search(pos, end, m, reg); pos = m.suffix().first)
	{
		if (m.prefix().length())
		{
			AMessage a;
			a.type = AMessage::Plain;
			a.Plain_text = m.prefix().str();
			mc.chain.push_back(a);
		}

		std::string b64decode;
		auto d = base64::decode(m.str(1));
		b64decode.assign(d.begin(), d.end());
		Json::Value v = parseJson(b64decode);
		mc.chain.push_back(AMessage::fromJson(v));

	}
	if (pos != end)
	{
		AMessage a;
		a.type = AMessage::Plain;
		a.Plain_text = std::string(pos, end);
		mc.chain.push_back(a);
	}
	//std::cout << dumpsJson(mc.toJson()) << "\n";
	
	return mc;
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
		v["type"] = "GroupMessage";
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
	//std::cout << v["type"].asString().c_str() << "\n";
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

AMessage::AMessage()
{
}

AMessage::~AMessage()
{
}

Json::Value AMessage::toJson()
{
	Json::Value v;
	switch (type)
	{
	case AMessage::Source:
		v["type"] = "Source";
		v["id"] = Source_id;
		v["time"] = Source_time;
		break;

	case AMessage::Quote:
		v["type"] = "Quote";
		v["id"] = Quote_id;
		v["groupId"] = Quote_groupId;
		v["senderId"] = Quote_senderId;
		v["targetId"] = Quote_targetId;
		v["origin"] = (*Quote_origin).toJson();
		break;

	case AMessage::At:
		break;
	case AMessage::AtAll:
		break;
	case AMessage::Face:
		break;
	case AMessage::Plain:
		v["type"] = "Plain";
		v["text"] = Plain_text;
		break;

	case AMessage::Image:
		v["type"] = "Image";
		v["imageId"] = Image_imageId;
		v["url"] = Image_url;
		v["path"] = Image_path;
		break;

	case AMessage::FlashImage:
		break;
	case AMessage::Voice:
		break;
	case AMessage::Xml:
		break;
	case AMessage::Json:
		break;
	case AMessage::App:
		break;
	case AMessage::Poke:
		break;
	case AMessage::Forward:
		v["type"] = "Forward";
		v["title"] = Forward_title;
		v["brief"] = Forward_brief;
		v["source"] = Forward_source;
		v["summary"] = Forward_summary;
		for (int i = 0; i < Forward_nodeList.size(); i++)
		{
			v["nodeList"][i]["senderId"] = Forward_nodeList[i].senderId;
			v["nodeList"][i]["time"] = Forward_nodeList[i].time;
			v["nodeList"][i]["senderName"] = Forward_nodeList[i].senderName;
			v["nodeList"][i]["messageChain"] = (*(Forward_nodeList[i].messageChain)).toJson();
		}
		break;

	case AMessage::File:
		break;
	default:
		break;
	}

	return v;
}

AMessage AMessage::fromJson(Json::Value v)
{
	AMessage a;
	switch (stringhash_run_time(v["type"].asString().c_str()))
	{
	case "Source"_hash:
		a.type = AMessage::Source;
		a.Source_id = v["id"].asInt64();
		a.Source_time = v["time"].asInt64();
		break;

	case "Quote"_hash:
		a.type = AMessage::Quote;
		a.Quote_id = v["id"].asInt64();
		a.Quote_groupId = v["groupId"].asInt64();
		a.Quote_senderId = v["senderId"].asInt64();
		a.Quote_targetId = v["targetId"].asInt64();
		a.Quote_origin = std::make_shared<MessageChain>();
		*(a.Quote_origin) = MessageChain::fromJson(v["origin"]);
		break;

	case "Plain"_hash:
		a.type = AMessage::Plain;
		a.Plain_text = v["text"].asString();
		break;

	case "Image"_hash:
		a.type = AMessage::Image;
		a.Image_imageId = v["imageId"].asString();
		a.Image_url = v["url"].asString();
		a.Image_path = v["path"].asString();
		break;

	case "Forward"_hash:
		a.type = AMessage::Forward;
		a.Forward_title = v["title"].asString();
		a.Forward_brief = v["brief"].asString();
		a.Forward_source = v["source"].asString();
		a.Forward_summary = v["summary"].asString();
		for (int i = 0; i < v["nodeList"].size(); i++)
		{
			Node n;
			n.senderId = v["nodeList"][i]["senderId"].asInt64();
			n.time = v["nodeList"][i]["time"].asInt64();
			n.senderName = v["nodeList"][i]["senderName"].asString();
			n.messageChain = std::make_shared<MessageChain>();
			*(n.messageChain) = MessageChain::fromJson(v["nodeList"][i]["messageChain"]);

			a.Forward_nodeList.push_back(n);
		}
		break;

	default:
		break;
	}

	return a;
}
