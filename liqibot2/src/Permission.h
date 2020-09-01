#pragma once
#include "Tools.h"
#include "web_api/api_mirai_http.h"

class Permission
{
public:

	enum Role
	{
		DefaultRole,
		VIP,
		SVIP,
		Admin,
	};

	Permission();
	~Permission();

	void saveConfig();
	void loadConfig();

	void onCommand(Message msg, std::string s, QQApi* qqApi_ptr);
	Role getRole(int64 group, int64 qq);
	int64 getTimeLeft(int64 group, int64 qq);
	void setRole(int64 group, int64 qq, Role role);
	void setRole(int64 group, int64 qq, Role role, int64 seconds);

	Json::Value config;
	std::string config_path = "data/PermissionConfig.json";

private:

};
