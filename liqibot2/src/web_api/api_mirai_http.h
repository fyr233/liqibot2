#pragma once
#include <string>
#include <vector>
#include <functional>

#include <json/json.h>

#include "../Message.h"


class QQApi
{
public:
	QQApi();
	~QQApi();

	Json::Value about();
	int connect();
	int auth();
	int verify();
	int release();

	int64 sendMessage(Member member, int64 quote, MessageChain msgChain);
	int64 sendFriendMessage(int64 qq, int64 quote, MessageChain msgChain);
	int64 sendFriendMessage(Member member, int64 quote, MessageChain msgChain);
	int64 sendTempMessage(int64 group, int64 qq, int64 quote, MessageChain msgChain);
	int64 sendTempMessage(Group group, Member member, int64 quote, MessageChain msgChain);
	int64 sendGroupMessage(int64 group, int64 quote, MessageChain msgChain);
	int64 sendGroupMessage(Group group, int64 quote, MessageChain msgChain);
	int64 sendImageMessage();//TODO
	int64 uploadImage();//TODO
	int recall(int64 msgId);

	std::vector<Member> getFriendList();
	std::vector<Group> getGroupList();
	std::vector<Member> getMemberList();

	int muteAll(Group group);
	int unmuteAll(Group group);
	int mute(Group group, Member member, int sec);
	int unmute(Group group, Member member);
	int kick();//TODO
	int quit(Group group);

	int setGroupConfig();//TODO
	Json::Value getGroupConfig(int64 group);
	Json::Value getGroupConfig(Group group);
	int setMemberInfo();//TODO
	Json::Value getMemberInfo(int64 group, int64 qq);
	Json::Value getMemberInfo(Group group, Member member);
	int setSessionConfig(int cacheSize, bool enableWS);
	Json::Value getSessionConfig();

	void startListen(std::function<void(Message msg, QQApi* qqApi)> onReceived);

	int port = 8080;
	std::string host = "localhost";
	std::string http_url = std::string("http://") + host + ":" + std::to_string(port);
	std::string ws_url = std::string("ws://") + host + ":" + std::to_string(port);
	std::string authKey = "123456789";
	std::string sessionKey = "";
	int64 qq = 2155679839;


private:
	std::function<void(Message msg, QQApi* qqApi)> onReceived;

	void onHandshake();
	void onMessage(std::string s);
	void onErr(std::string s);
	void onClose();

	Json::Value parseJson(std::string s);
	std::string dumpsJson(Json::Value v);
};



