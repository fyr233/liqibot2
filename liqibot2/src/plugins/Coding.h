#pragma once

#include "../web_api/api_mirai_http.h"
#include "Plugin.h"

class Coding: Plugin
{
public:
	Coding();
	Coding(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr);
	~Coding();

	float metric(Message msg);
	void run(Message msg, QQApi* qqApi_ptr);
	void onCommand(Message msg, std::string s, QQApi* qqApi_ptr);
	void onClose();

private:

};
