#pragma once
#include "../vendor/json/json.hpp"

class JsonReader
{
public:
	static nlohmann::json_abi_v3_11_3::basic_json<nlohmann::json_abi_v3_11_3::ordered_map> ReadJson(const char* path);
};