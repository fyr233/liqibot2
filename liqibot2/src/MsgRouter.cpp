#pragma once

//ERROR C4996
#define _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING

#include "MsgRouter.h"
#include "Message.h"
#include "web_api/api_mirai_http.h"
#include "plugins/Plugin.h"
#include "plugins/Repeat.h"
#include "plugins/Command.h"
#include "plugins/ScoldMe.h"
#include "plugins/Default.h"
#include "plugins/RandomReply.h"
#include "plugins/Setu.h"

MsgRouter::MsgRouter()
{
	rt_table_static = {
		(Plugin*) new Repeat(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new Command(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new ScoldMe(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new Default(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new RandomReply(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new Setu(&rt_table_dynamic, &rt_table_static, &permission)
	};
}

MsgRouter::~MsgRouter()
{
	
}

void MsgRouter::onReceived(Message msg, QQApi* qqApi_ptr)
{
	//msg.msgChain.print();

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
		Plugin* p = rt_table_static[max_id];
		thread_pool.enqueue(
			[p, msg, qqApi_ptr] {
				try
				{
					p->run(msg, qqApi_ptr);
				}
				catch (const std::exception&)
				{
					std::cout << "plugin " + p->name + " throw an error\n";
				}
				
			}
		);
	}
}
