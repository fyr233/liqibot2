#pragma once



#include "../Message.h"
#include "../web_api/api_mirai_http.h"
#include "../Permission.h"

class Plugin
{
public:
	Plugin() {}
	Plugin(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr) {}
	~Plugin() {}

	virtual float metric(Message msg);	//用于消息路由
	virtual void run(Message msg, QQApi* qqApi_ptr);	//消息入口
	virtual void onCommand(Message msg, std::string s, QQApi* qqApi_ptr);	//命令入口
	void saveConfig();	//保存config文件
	void loadConfig();	//加载config文件
	//virtual void onClose();	//即将关闭时调用

	std::string name = "Plugin";	//插件名称
	Json::Value config;
	std::vector<Plugin*>* rt_tb_dy_ptr;
	std::vector<Plugin*>* rt_tb_st_ptr;
	Permission* permission_ptr;

private:

};

inline float Plugin::metric(Message msg)
{
	return 0.0;
}

inline void Plugin::run(Message msg, QQApi* qqApi_ptr)
{
	return;
}

inline void Plugin::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
}

inline void Plugin::saveConfig()
{
	std::ofstream config_file("data/plugins/" + name + "/config.json");
	config_file << dumpsJson(config);
	config_file.close();
}

inline void Plugin::loadConfig()
{
	try
	{
		config = loadJson("data/plugins/" + name + "/config.json");
	}
	catch (const std::string& e)//解析失败，使用默认config
	{
		std::cout << "Load " + std::string("data/plugins/") + name + "/config.json failed.\n"
			<< e << "\n"
			<< "Using default config" << "\n";
	}
}

