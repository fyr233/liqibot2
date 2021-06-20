﻿#pragma once
#include <json/json.h>

#include "api_mirai_http.h"
#include "../Requests.h"
#include "../WebSocket.h"
#include "../Log.h"

QQApi::QQApi()
{
	//默认config
	std::string conf = R""(
		{
			"qq": 2155679839,
			"host": "localhost",
			"port": 8080,
			"verifyKey": "123456789"
		}
		)"";

	config = parseJson(conf);
	loadConfig();
	saveConfig();

	qq = config["qq"].asInt64();
	host = config["host"].asString();
	port = config["port"].asInt();
	verifyKey = config["verifyKey"].asString();
	http_url = std::string("http://") + host + ":" + std::to_string(port);
	ws_url = std::string("ws://") + host + ":" + std::to_string(port);
}

QQApi::~QQApi()
{
}

void QQApi::saveConfig()
{
	std::ofstream config_file(config_path);
	config_file << dumpsJson(config);
	config_file.close();
}

void QQApi::loadConfig()
{
	try
	{
		config = loadJson(config_path);
	}
	catch (const std::string& e)//解析失败，使用默认config
	{
		std::cout << "Load " + config_path + " failed.\n"
			<< e
			<< "Using default config" << "\n\n";
	}
}

Json::Value QQApi::about()
{
	Requests r = Requests::get(http_url + "/about");

	return parseJson(r.text);
}

int QQApi::connect()
{
	if (verify() == 0)
	{
		if (bind() == 0)
		{
			return 0;
		}
	}
	return -1;
}

int QQApi::verify()
{
	Json::Value d;
	d["verifyKey"] = verifyKey;

	Requests r = Requests::post(http_url + "/verify", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		if (res["code"].asInt() == 0)
		{
			sessionKey = res["session"].asString();
		}

		return res["code"].asInt();
	}

	return -1;
}

int QQApi::bind()
{
	Json::Value d;
	d["sessionKey"] = sessionKey;
	d["qq"] = this->qq;

	Requests r = Requests::post(http_url + "/bind", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		if (res["code"].asInt() == 0)
		{
			std::cout << "bind success" << "\n";
			return 0;
		}
		return res["code"].asInt();
	}

	return -1;
}

int QQApi::release()
{
	Json::Value d;
	d["sessionKey"] = sessionKey;
	d["qq"] = qq;

	Requests r = Requests::post(http_url + "/release", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		if (res["code"].asInt() == 0)
		{
			std::cout << "release success" << "\n";
			return 0;
		}
		return res["code"].asInt();
	}

	return -1;
}

int64 QQApi::sendMessage(Member member, int64 quote, std::string s)
{
	return sendMessage(member, quote, MessageChain::fromString(s));
}

int64 QQApi::sendMessage(Member member, int64 quote, MessageChain msgChain)
{
	if (member.group.id == 0)
	{
		return sendFriendMessage(member, quote, msgChain);
	}
	else
	{
		return sendGroupMessage(member.group, quote, msgChain);
	}
}

int64 QQApi::sendFriendMessage(int64 qq, int64 quote, MessageChain msgChain)
{
	Json::Value d;
	d["sessionKey"] = sessionKey;
	d["qq"] = qq;
	d["messageChain"] = msgChain.toJson();
	if (quote != 0)
	{
		d["quote"] = quote;
	}

	Requests r = Requests::post(http_url + "/sendFriendMessage", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		if (res["code"].asInt() == 0)
		{
			Log::add("send", dumpsJson(d, false), this->qq);
			return res["messageId"].asInt64();
		}
		return res["code"].asInt();
	}

	return -1;
}

int64 QQApi::sendFriendMessage(Member member, int64 quote, MessageChain msgChain)
{
	return sendFriendMessage(member.id, quote, msgChain);
}

int64 QQApi::sendTempMessage(int64 group, int64 qq, int64 quote, MessageChain msgChain)
{
	Json::Value d;
	d["sessionKey"] = sessionKey;
	d["qq"] = qq;
	d["group"] = group;
	d["messageChain"] = msgChain.toJson();
	if (quote != 0)
	{
		d["quote"] = quote;
	}

	Requests r = Requests::post(http_url + "/sendTempMessage", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		if (res["code"].asInt() == 0)
		{
			Log::add("send", dumpsJson(d, false), this->qq);
			return res["messageId"].asInt64();
		}
		return res["code"].asInt();
	}

	return -1;
}

int64 QQApi::sendTempMessage(Group group, Member member, int64 quote, MessageChain msgChain)
{
	return sendTempMessage(group.id, member.id, quote, msgChain);
}

int64 QQApi::sendGroupMessage(int64 group, int64 quote, MessageChain msgChain)
{
	Json::Value d;
	d["sessionKey"] = sessionKey;
	d["group"] = group;
	d["messageChain"] = msgChain.toJson();
	if (quote != 0)
	{
		d["quote"] = quote;
	}

	//std::cout << dumpsJson(d, false) << std::endl;
	Requests r = Requests::post(http_url + "/sendGroupMessage", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		if (res["code"].asInt() == 0)
		{
			Log::add("send", dumpsJson(d, false), this->qq);
			return res["messageId"].asInt64();
		}
		else
		{
			std::cout << res["msg"] << std::endl;
		}
		return res["code"].asInt();
	}

	return -1;
}

int64 QQApi::sendGroupMessage(Group group, int64 quote, MessageChain msgChain)
{
	return sendGroupMessage(group.id, quote, msgChain);
}

int64 QQApi::sendImageMessage()
{
	return 0;
}

int64 QQApi::uploadImage()
{
	return 0;
}

int QQApi::recall(int64 msgId)
{
	Json::Value d;
	d["sessionKey"] = sessionKey;
	d["target"] = msgId;
	
	Requests r = Requests::post(http_url + "/recall", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		if (res["code"].asInt() == 0)
		{
			Log::add("send", dumpsJson(d, false), this->qq);
			return 0;
		}
		return -1;
	}

	return -1;
}

std::vector<Member> QQApi::getFriendList()
{
	Json::Value d;
	d["sessionKey"] = sessionKey;

	Requests r = Requests::get(http_url + "/friendList", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		std::vector<Member> members;
		for (int i = 0; i < res.size(); i++)
		{
			Member m;
			m.id = res[i]["id"].asInt64();
			m.nickname = res[i]["nickname"].asString();
			m.remark = res[i]["remark"].asString();
			members.push_back(m);
		}
		return members;
	}

	return std::vector<Member>();
}

std::vector<Group> QQApi::getGroupList()
{
	Json::Value d;
	d["sessionKey"] = sessionKey;

	Requests r = Requests::get(http_url + "/groupList", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		std::vector<Group> groups;
		for (int i = 0; i < res.size(); i++)
		{
			Group g;
			g.id = res[i]["id"].asInt64();
			g.name = res[i]["name"].asString();
			switch (stringhash_run_time(res[i]["permission"].asString().c_str()))
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
			
			groups.push_back(g);
		}
		return groups;
	}

	return std::vector<Group>();
}

std::vector<Member> QQApi::getMemberList()
{
	Json::Value d;
	d["sessionKey"] = sessionKey;

	Requests r = Requests::get(http_url + "/friendList", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		std::vector<Member> members;
		for (int i = 0; i < res.size(); i++)
		{
			Member m;
			m.id = res[i]["id"].asInt64();
			m.memberName = res[i]["memberName"].asString();
			switch (stringhash_run_time(res[i]["permission"].asString().c_str()))
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

			Group g;
			g.id = res[i]["id"]["group"]["id"].asInt64();
			g.name = res[i]["id"]["group"]["name"].asString();
			switch (stringhash_run_time(res[i]["id"]["group"]["permission"].asString().c_str()))
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

			m.group = g;
			members.push_back(m);
		}
		return members;
	}

	return std::vector<Member>();
}

int QQApi::muteAll(Group group)
{
	return 0;
}

int QQApi::unmuteAll(Group group)
{
	return 0;
}

int QQApi::mute(Group group, Member member, int sec)
{
	return 0;
}

int QQApi::unmute(Group group, Member member)
{
	return 0;
}

int QQApi::kick()
{
	return 0;
}

int QQApi::quit(Group group)
{
	return 0;
}

int QQApi::setGroupConfig()
{
	return 0;
}

Json::Value QQApi::getGroupConfig(int64 group)
{
	return Json::Value();
}

Json::Value QQApi::getGroupConfig(Group group)
{
	return Json::Value();
}

int QQApi::setMemberInfo()
{
	return 0;
}

Json::Value QQApi::getMemberInfo(int64 group, int64 qq)
{
	return Json::Value();
}

Json::Value QQApi::getMemberInfo(Group group, Member member)
{
	return Json::Value();
}

int QQApi::setSessionConfig(int cacheSize, bool enableWS)
{
	return 0;
}

Json::Value QQApi::getSessionConfig()
{
	return Json::Value();
}

void QQApi::startListen(std::function<void(std::string, QQApi* qqApi)> onReceived)
{
	this->onReceived = onReceived;
	auto ws = std::make_shared<WebSocket>(ws_url + "/all?" + "verifyKey=" + verifyKey + "&qq=" + std::to_string( qq),
		[this]() {this->onHandshake(); },
		[this](std::string s) {this->onMessage(s); }, 
		[this](std::string s) {this->onErr(s); }, 
		[this]() {this->onClose(); }
	);
	ws->start();
	ws->run();//阻塞的
}

void QQApi::onHandshake()
{
}

void QQApi::onMessage(std::string s)
{
	std::cout << s << "\n";
	
	(onReceived)(s, this);
}

void QQApi::onErr(std::string s)
{
}

void QQApi::onClose()
{
}

