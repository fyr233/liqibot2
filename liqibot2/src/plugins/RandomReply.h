#pragma once
#include <random>

#include <json.h>
#include "../web_api/api_mirai_http.h"
#include "Plugin.h"

class RandomReply : Plugin
{
public:
	RandomReply();
	RandomReply(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr);
	~RandomReply();

	float metric(Message msg);
	void run(Message msg, QQApi* qqApi_ptr);
	void onCommand(Message msg, std::string s, QQApi* qqApi_ptr);
	void onClose();

private:

	int rand_max = 100000;
	std::random_device rd;   // non-deterministic generator  
	std::mt19937 gen = std::mt19937(rd());  // to seed mersenne twister.  
	std::uniform_int_distribution<> dist = std::uniform_int_distribution<>(0, rand_max); // distribute results between 1 and 6 inclusive. 

};

