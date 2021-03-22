﻿#pragma once
#include <random>

#include <json.h>
#include "../web_api/api_mirai_http.h"
#include "Plugin.h"

class ScoldMe : Plugin
{
public:
	ScoldMe();
	ScoldMe(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr);
	~ScoldMe();

	float metric(Message msg);
	void run(Message msg, QQApi* qqApi_ptr);
	void onCommand(Message msg, std::string s, QQApi* qqApi_ptr);
	void onClose();

private:
	std::string logfolderpath = "data/plugins/ScoldMe/log/";

	void addlog(std::string sendorrecv, Member m, std::string msg);
	
};