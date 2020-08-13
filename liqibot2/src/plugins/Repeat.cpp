#pragma once
#include "repeat.h"
#include "../web_api/api_mirai_http.h"

Repeat::Repeat()
{
}

Repeat::Repeat(std::vector<Plugin*>* rt_tb_dy_ptr)
{
	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
}

Repeat::~Repeat()
{
}

float Repeat::metric(Message msg)
{
	if (msg.member.id == 1325275429LL)
	{
		return 1.0;
	}
	return 0.0;
}

void Repeat::run(Message msg, QQApi* qqApi_ptr)
{
	qqApi_ptr->sendMessage(msg.member, 0, msg.msgChain);
}