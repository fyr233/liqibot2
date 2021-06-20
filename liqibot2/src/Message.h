#pragma once
#include <string>
#include <vector>
#include <iostream>

#include <json/json.h>

#include "Tools.h"

class MessageChain;
class AMessage;

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

class AMessage
{
public:
	enum Type
	{
		Source,
		Quote,
		At,
		AtAll,
		Face,
		Plain,
		Image,
		FlashImage,
		Voice,
		Xml,
		Json,
		App,
		Poke,
		Forward,
		File
	};

	Type type;

	AMessage();
	~AMessage();
	Json::Value toJson();
	static AMessage fromJson(Json::Value v);
	std::string toString();
	static AMessage fromString(std::string s);

	int64 Source_id;
	int64 Source_time;

	int64 Quote_id;
	int64 Quote_groupId;
	int64 Quote_senderId;
	int64 Quote_targetId;
	std::shared_ptr<MessageChain> Quote_origin = nullptr;

	int64 At_target;
	std::string At_display;

	int64 Face_faceId;
	std::string Face_name;

	std::string Plain_text;

	std::string Image_imageId;
	std::string Image_url;
	std::string Image_path;

	std::string FlashImage_imageId;
	std::string FlashImage_url;
	std::string FlashImage_path;

	std::string Voice_voiceId;
	std::string Voice_url;
	std::string Voice_path;

	std::string Xml_xml;

	std::string Poke_name;

	struct Node
	{
		int64 senderId;
		int64 time;
		std::string senderName;
		std::shared_ptr<MessageChain> messageChain;
	};

	std::string Forward_title;
	std::string Forward_brief;
	std::string Forward_source;
	std::string Forward_summary;
	std::vector<Node> Forward_nodeList;

	std::string File_id;
	int64 File_internalId;
	std::string File_name;
	int64 File_size;
};

class MessageChain
{
public:
	MessageChain() {}
	~MessageChain() {}

	void print();
	Json::Value toJson();
	static MessageChain fromJson(Json::Value v);
	std::string toString();
	static MessageChain fromString(std::string s);

public: 

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
