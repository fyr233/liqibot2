#pragma once
#include "Tools.h"


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

