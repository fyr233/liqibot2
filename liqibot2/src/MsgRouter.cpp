﻿#pragma once

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
#include "plugins/Tex.h"
#include "plugins/Statistics.h"
#include "plugins/Coding.h"
#include "plugins/YuanShen.h"

MsgRouter::MsgRouter()
{
	rt_table_static = {
		(Plugin*) new Repeat(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new Command(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new ScoldMe(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new Default(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new RandomReply(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new Setu(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new Tex(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new Statistics(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new Coding(&rt_table_dynamic, &rt_table_static, &permission),
		(Plugin*) new YuanShen(&rt_table_dynamic, &rt_table_static, &permission)
	};

	std::cout << "ThreadPool size: " << 2 * std::thread::hardware_concurrency() << "\n";
}

MsgRouter::~MsgRouter()
{
	
}

void MsgRouter::onReceived(std::string s, QQApi* qqApi_ptr)
{

	Message msg = Message::fromJson(parseJson(s)["data"]);
	//msg.msgChain.print();
	//std::cout << parseJson(s) << "\n";

	switch (msg.type)
	{
	case Message::FriendMessage:
	case Message::GroupMessage:
		Log::add("recv", s, qqApi_ptr->qq);
		break;
	default:
		Log::add("event", s, qqApi_ptr->qq);
		break;
	}

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
				catch (const std::exception& e)
				{
					std::cout << "plugin " + p->name + " throw an error\n";
						//<< std::to_string(e) << "\n";
				}
				
			}
		);
	}
}
