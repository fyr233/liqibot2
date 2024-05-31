#include "Yinglish.h"

#include "../SubProcess.h"

Yinglish::Yinglish()
{
}

Yinglish::Yinglish(std::vector<Plugin*>* rt_tb_dy_ptr, std::vector<Plugin*>* rt_tb_st_ptr, Permission* permission_ptr)
{
	//��Ҫ��ģ������
	Plugin::name = "Yinglish";

	this->rt_tb_dy_ptr = rt_tb_dy_ptr;
	this->rt_tb_st_ptr = rt_tb_st_ptr;
	this->permission_ptr = permission_ptr;

	//Ĭ��config
	std::string conf = u8R""(
		{
			"active": true,
			"triggers": [
				"˵",
				]
		}
		)"";

	config = parseJson(conf);
	loadConfig();
}

Yinglish::~Yinglish()
{
}

float Yinglish::metric(Message msg)
{
	//�жϿ�ͷ�Ƿ��Ǵ�����
	if (config["active"].asBool())
	{
		if (msg.type == Message::FriendMessage || msg.type == Message::GroupMessage)
		{
			std::string s = msg.msgChain.toString();

			for (int i = 0; i < config["triggers"].size(); i++)
			{
				std::string command = config["triggers"][i].asString();
				//std::cout << s << "\n" << command << "\n" << (s == command) << "\n";
				if (s.substr(0, command.size()) == command)
				{
					return 1.0;
				}
			}
		}
	}
	return 0.0f;
}

void Yinglish::run(Message msg, QQApi* qqApi_ptr)
{
	std::string s = msg.msgChain.toString();
	for (int i = 0; i < config["triggers"].size(); i++)
	{
		std::string command = config["triggers"][i].asString();
		//std::cout << s << "\n" << command << "\n" << (s == command) << "\n";
		if (s.substr(0, command.size()) == command)
		{
			s = s.substr(command.size());
			break;
		}
	}

	s = utf8_to_ansi(s);
	//std::cout << tex << "\n";

	//����Yinglish.py������s������ת�����ַ���
	std::string ans = SubProcess::popen("python data/plugins/Yinglish/Yinglish.py \"" + s + "\"");
	ans = ansi_to_utf8(ans);

	//������Ϣ
	qqApi_ptr->sendMessage(msg.member, 0, ans);
	std::cout << "Yinglish: " << ans << "\n";
}

void Yinglish::onCommand(Message msg, std::string s, QQApi* qqApi_ptr)
{
	/*
		example: plugins/Yinglish -c add -t ˵
				 /plugins/ScoldMe -c get
				 plugins/ScoldMe -c get --json
				 plugins/ScoldMe -c del -t ˵
	*/


}

void Yinglish::onClose()
{
}
