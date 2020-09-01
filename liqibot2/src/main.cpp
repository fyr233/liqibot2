#pragma once
#include <iostream>

#include <json/json.h>

#include "web_api/api_mirai_http.h"
#include "MsgRouter.h"

int main()
{
	//system("chcp 65001"); //设置字符集

	auto qqApi = QQApi();
	auto msgRouter_ptr = new MsgRouter();

	std::cout << "connect: " << qqApi.connect() << "\n";
	qqApi.startListen([msgRouter_ptr](Message msg, QQApi* qqApi) {msgRouter_ptr->onReceived(msg, qqApi); });

	delete msgRouter_ptr;

	return 0;
}