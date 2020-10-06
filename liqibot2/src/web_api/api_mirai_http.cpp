#pragma once
#include <json/json.h>

#include "api_mirai_http.h"
#include "../Requests.h"
#include "../WebSocket.h"


QQApi::QQApi()
{
	//默认config
	std::string conf = R""(
		{
			"qq": 2155679839,
			"host": "localhost",
			"port": 8080,
			"authKey": "123456789"
		}
		)"";

	config = parseJson(conf);
	loadConfig();
	saveConfig();

	qq = config["qq"].asInt64();
	host = config["host"].asString();
	port = config["port"].asInt();
	authKey = config["authKey"].asString();
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
	if (auth() == 0)
	{
		if (verify() == 0)
		{
			return 0;
		}
	}
	return -1;
}

int QQApi::auth()
{
	Json::Value d;
	d["authKey"] = authKey;

	Requests r = Requests::post(http_url + "/auth", dumpsJson(d));
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

int QQApi::verify()
{
	Json::Value d;
	d["sessionKey"] = sessionKey;
	d["qq"] = this->qq;

	Requests r = Requests::post(http_url + "/verify", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		if (res["code"].asInt() == 0)
		{
			std::cout << "verify success" << "\n";
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

	Requests r = Requests::post(http_url + "/sendGroupMessage", dumpsJson(d));
	if (r.code == 200)
	{
		Json::Value res = parseJson(r.text);
		if (res["code"].asInt() == 0)
		{
			return res["messageId"].asInt64();
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
			return 0;
		}
		return -1;
	}

	return -1;
}

std::vector<Member> QQApi::getFriendList()
{
	return std::vector<Member>();
}

std::vector<Group> QQApi::getGroupList()
{
	return std::vector<Group>();
}

std::vector<Member> QQApi::getMemberList()
{
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

void QQApi::startListen(std::function<void(Message msg, QQApi* qqApi)> onReceived)
{
	this->onReceived = onReceived;
	auto ws = std::make_shared<WebSocket>(ws_url + "/all?sessionKey=" + sessionKey,
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
	Message msg = Message::fromJson(parseJson(s));
	(onReceived)(msg, this);
}

void QQApi::onErr(std::string s)
{
}

void QQApi::onClose()
{
}

