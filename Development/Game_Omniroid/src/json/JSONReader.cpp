#include "JSONReader.h"
#include <iostream>
#include <fstream>
#include <istream>

nlohmann::json_abi_v3_11_3::basic_json<nlohmann::json_abi_v3_11_3::ordered_map> JsonReader::ReadJson(const char* path)
{
	std::ifstream i(path);
	auto json = nlohmann::ordered_json::parse(i);
	i.clear();
	i.close();
	return json;
}
