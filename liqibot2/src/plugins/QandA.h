#pragma once
#include <random>

#include "../web_api/api_mirai_http.h"
#include "Plugin.h"

class QandA: Plugin
{
public:
	QandA();
	QandA(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr);
	~QandA();

	float metric(Message msg);
	void run(Message msg, QQApi* qqApi_ptr);
	void onCommand(Message msg, std::string s, QQApi* qqApi_ptr);
	void onClose();

private:

};