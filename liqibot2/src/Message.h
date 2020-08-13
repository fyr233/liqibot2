#pragma once
#include <string>
#include <vector>
#include <iostream>

#include <json/json.h>

using int64 = long long;

class Group
{
public:
	Group() {}
	~Group() {}

	void print();
	Json::Value toJson();
	static Group fromJson(Json::Value v);

	enum Permission
	{
		MEMBER,
		ADMINISTRATOR,
		OWNER,
	};

	int64 id = 0;
	std::string name;
	Permission permission = MEMBER;

private:

};

class Member
{
public:
	Member() {}
	~Member() {}

	void print();
	Json::Value toJson();
	static Member fromJson(Json::Value v);

	enum Permission
	{
		MEMBER,
		ADMINISTRATOR,
		OWNER,
	};

	int64 id = 0;
	std::string memberName;
	std::string nickname;
	std::string remark;
	Permission permission = MEMBER;
	Group group;

private:

};

class MessageChain
{
public:
	MessageChain() {}
	~MessageChain() {}

	void print();
	Json::Value toJson();
	static MessageChain fromJson(Json::Value v);
	static MessageChain fromString(std::string s);

	struct AMessage
	{
		enum Type
		{
			Source,
			Plain,
			Image,
		};

		Type type;

		int64 id;
		int64 time;

		std::string text;

		std::string imageId;
		std::string url;
		std::string path;


	};

	std::vector<AMessage> chain;

private:

};

class Message
{
public:
	Message() {}
	~Message() {}

	void print();
	Json::Value toJson();
	static Message fromJson(Json::Value v);

	enum Type
	{
		None,
		FriendMessage,
		GroupMessage,
	};


	Type type = None;
	Member member;
	MessageChain msgChain;

private:

};
