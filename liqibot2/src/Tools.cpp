#pragma once
#include "Tools.h"
#include <time.h>
#include <codecvt>
#include <Windows.h>

hash_t stringhash_run_time(const char* str, hash_t last_value) {
	return *str ? stringhash_run_time(str + 1, (*str ^ last_value) * stringhash_prime) : last_value;
}

Json::Value parseJson(std::string s)
{
	bool res;
	JSONCPP_STRING errs;
	Json::Value root, lang, mail;
	Json::CharReaderBuilder readerBuilder;
	readerBuilder.settings_["emitUTF8"] = true;

	std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
	res = jsonReader->parse(s.c_str(), s.c_str() + s.length(), &root, &errs);
	if (!res || !errs.empty()) {
		//std::cout << "parseJson err. " << errs << std::endl;
		throw "parseJson err. " + errs;
	}

	return root;
}

std::string dumpsJson(Json::Value v)
{
	Json::StreamWriterBuilder writerBuilder;
	writerBuilder.settings_["emitUTF8"] = true;
	std::ostringstream os;
	std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
	jsonWriter->write(v, &os);

	return os.str();
}

Json::Value loadJson(std::string file)
{
	JSONCPP_STRING errs;
	Json::Value root, lang, mail;
	Json::CharReaderBuilder readerBuilder;
	readerBuilder["collectComments"] = true;
	readerBuilder.settings_["emitUTF8"] = true;

	std::ifstream ifs;
	ifs.open(file);
	
	if (!parseFromStream(readerBuilder, ifs, &root, &errs)) {
		throw "parseJson err. " + errs;
	}

	return root;
}

std::vector<std::string> splitString(const std::string& s, const std::string& spliter)
{
	std::string::size_type pos1, pos2;
	std::vector<std::string> v;
	pos2 = s.find(spliter);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + spliter.size();
		pos2 = s.find(spliter, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
	return v;
}

std::string utf8_to_ansi(std::string strUTF8) {
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
	WCHAR* wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR* szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strUTF8 = szBuffer;
	delete[]szBuffer;
	delete[]wszBuffer;
	return strUTF8;
}

std::string ansi_to_utf8(std::string strAnsi) {
	UINT nLen = MultiByteToWideChar(936, NULL, strAnsi.c_str(), -1, NULL, NULL);
	WCHAR* wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(936, NULL, strAnsi.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR* szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strAnsi = szBuffer;
	delete[]wszBuffer;
	delete[]szBuffer;
	return strAnsi;
}

char* utf8_to_ansi(char* strUTF8A) {
	std::string strUTF8 = strUTF8A;
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
	WCHAR* wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR* szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strUTF8 = szBuffer;
	delete[]szBuffer;
	delete[]wszBuffer;
	char* p = (char*)strUTF8.data();
	return p;
}

char* ansi_to_utf8(char* strAnsiA) {
	std::string strAnsi = strAnsiA;
	UINT nLen = MultiByteToWideChar(936, NULL, strAnsi.c_str(), -1, NULL, NULL);
	WCHAR* wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(936, NULL, strAnsi.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR* szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strAnsi = szBuffer;
	delete[]wszBuffer;
	delete[]szBuffer;
	char* p = (char*)strAnsi.data();
	return p;
}