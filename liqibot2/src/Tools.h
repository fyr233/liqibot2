#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>

#include <cppcodec/base32_crockford.hpp>
#include <cppcodec/base64_rfc4648.hpp>

#include <json/json.h>


using int64 = long long;

using hash_t = size_t;
constexpr hash_t stringhash_prime = 0x100000001B3ull;
constexpr hash_t stringhash_basis = 0xCBF29CE484222325ull;

using base32 = cppcodec::base32_crockford;
using base64 = cppcodec::base64_rfc4648;

constexpr hash_t stringhash_compile_time(const char* str, hash_t last_value = stringhash_basis) {
	return *str ? stringhash_compile_time(str + 1, (*str ^ last_value) * stringhash_prime) : last_value;
}
constexpr hash_t operator "" _hash(const char* p, size_t) {
	return stringhash_compile_time(p);
}

hash_t stringhash_run_time(const char* str, hash_t last_value = stringhash_basis);

Json::Value parseJson(std::string s);
std::string dumpsJson(Json::Value v, bool multiline=true);
Json::Value loadJson(std::string file);

std::vector<std::string> splitString(const std::string& s, const std::string& spliter);

std::string utf8_to_ansi(std::string);
std::string ansi_to_utf8(std::string);
char* utf8_to_ansi(char*);
char* ansi_to_utf8(char*);

