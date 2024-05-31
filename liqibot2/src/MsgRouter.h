#pragma once
#include <vector>

#include <ThreadPool.h>

#include "Message.h"
#include "web_api/api_mirai_http.h"
#include "plugins/Plugin.h"
#include "plugins/Repeat.h"
#include "Log.h"


class MsgRouter
{
public:
	MsgRouter();
	~MsgRouter();

	void onReceived(std::string s, QQApi* qqApi_ptr);
	void onClose();

	std::vector<Plugin*> rt_table_static;
	std::vector<Plugin*> rt_table_dynamic;
	Permission permission;

private:
	const int thread_pool_size = 64;
	ThreadPool thread_pool = ThreadPool(thread_pool_size);

};


