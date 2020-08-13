#pragma once
#include <vector>

#include "Message.h"
#include "web_api/api_mirai_http.h"
#include "plugins/Plugin.h"
#include "plugins/Repeat.h"


class MsgRouter
{
public:
	MsgRouter();
	~MsgRouter();

	void onReceived(Message msg, QQApi* qqApi_ptr);

	std::vector<Plugin*> rt_table_static;
	std::vector<Plugin*> rt_table_dynamic;

private:

};


