#include "Utils.h"

std::vector<uint8_t> Utils::ToData(const std::string& String)
{
	return std::move(std::vector<uint8_t>(String.begin(), String.end()));
}

