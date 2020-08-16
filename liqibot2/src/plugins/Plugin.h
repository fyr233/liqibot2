#pragma once

#include "../Message.h"
#include "../web_api/api_mirai_http.h"

class Plugin
{
public:
	Plugin() {}
	Plugin(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr) {}
	~Plugin() {}

	virtual float metric(Message msg);
	virtual void run(Message msg, QQApi* qqApi_ptr);
	virtual void onCommand(Message msg, std::string s, QQApi* qqApi_ptr);

	Json::Value config;
	std::vector<Plugin*>* rt_tb_dy_ptr;
	std::vector<Plugin*>* rt_tb_st_ptr;

private:

};

inline float Plugin::metric(Message msg)
{
	return 0.0;
}

inline void Plugin::run(Message msg, QQApi* qqApi_ptr)
{
	return;
}

inline void Plugin::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
}
