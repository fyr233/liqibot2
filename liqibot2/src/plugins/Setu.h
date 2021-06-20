#pragma once
#include <ctime>

#include <json.h>
#include "../web_api/api_mirai_http.h"
#include "Plugin.h"

class Setu: Plugin
{
public:
	Setu();
	Setu(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr);
	~Setu();

	float metric(Message msg);
	void run(Message msg, QQApi* qqApi_ptr);
	void onCommand(Message msg, std::string s, QQApi* qqApi_ptr);
	void onClose();

private:
	std::vector<time_t> timelist;
	std::string logfolderpath = "data/plugins/Setu/log/";

	void addlog(std::string sendorrecv, Member m, std::string image);

	std::string checkSetu(MessageChain Chain, Member member, QQApi* qqApi_ptr);
};
