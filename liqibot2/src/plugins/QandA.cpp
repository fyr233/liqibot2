#include "QandA.h"

#include "cmdline.h"

QandA::QandA()
{
}

QandA::QandA(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//重要，模块名字
	Plugin::name = "QandA";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//默认config
	std::string conf = u8R""(
		{
			"active": true,
			"trigger-reply": {
				
				}
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

QandA::~QandA()
{
}

float QandA::metric(Message msg)
{
	return 0.0f;
}

void QandA::run(Message msg, QQApi* qqApi_ptr)
{
}

void QandA::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
}

void QandA::onClose()
{
}
