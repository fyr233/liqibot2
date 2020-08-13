#pragma once
#include "../web_api/api_mirai_http.h"
#include "Plugin.h"

class Repeat: Plugin
{
public:
	Repeat();
	Repeat(std::vector<Plugin*> * rt_tb_dy_ptr);
	~Repeat();

	float metric(Message msg);

	void run(Message msg, QQApi* qqApi_ptr);

	std::vector<Plugin*>* rt_tb_dy_ptr;
	QQApi* qqApi_ptr;

private:

};

