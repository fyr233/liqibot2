#pragma once
#include <vector>

#include <ThreadPool.h>

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
	void onClose();

	std::vector<Plugin*> rt_table_static;
	std::vector<Plugin*> rt_table_dynamic;
	Permission permission;

private:

	ThreadPool thread_pool = ThreadPool(std::thread::hardware_concurrency());

};


