#pragma once
#include <iostream>

#include <json/json.h>

#include "web_api/api_mirai_http.h"
#include "MsgRouter.h"

int main()
{
	system("chcp 65001"); //设置字符集

	auto qqApi = QQApi();
	auto msgRouter_ptr = new MsgRouter();

	std::cout << "connect: " << qqApi.connect() << "\n";
	qqApi.startListen([msgRouter_ptr](std::string s, QQApi* qqApi) {msgRouter_ptr->onReceived(s, qqApi); });

	delete msgRouter_ptr;

	return 0;
}