#pragma once
#include "MsgRouter.h"
#include "Message.h"
#include "web_api/api_mirai_http.h"
#include "plugins/Plugin.h"
#include "plugins/Repeat.h"

MsgRouter::MsgRouter()
{
	rt_table_static = {
		(Plugin*) new Repeat(&rt_table_dynamic),

	};
}

MsgRouter::~MsgRouter()
{
}

void MsgRouter::onReceived(Message msg, QQApi* qqApi_ptr)
{
	msg.msgChain.print();

	float metric_max = 0.0;
	int max_id;
	for (int i = 0; i < rt_table_static.size(); i++)
	{
		float metric = rt_table_static[i]->metric(msg);
		if (metric > metric_max)
		{
			max_id = i;
			metric_max = metric;
		}
	}

	if (metric_max >= 0.5)
	{
		rt_table_static[max_id]->run(msg, qqApi_ptr);
	}
}
