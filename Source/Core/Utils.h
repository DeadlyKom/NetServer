#pragma once

#include <CoreMinimal.h>
#pragma warning(disable : 4996)				//_CRT_SECURE_NO_WARNINGS

#define FORMAT_BUFFER_SIZE 512

namespace Utils
{
	template <typename... Args>
	std::wstring Format(const std::wstring& format, Args... args)
	{
		wchar_t Buffer[FORMAT_BUFFER_SIZE];
		const size_t size = swprintf(Buffer, FORMAT_BUFFER_SIZE, format.c_str(), args...) + 1;
		return Buffer;
	}

	template <typename... Args>
	std::string Format(const std::string& format, Args... args)
	{
		char Buffer[FORMAT_BUFFER_SIZE];
		const size_t size = sprintf(Buffer, format.c_str(), args...) + 1;
		return Buffer;
	}

	std::vector<uint8_t> ToData(const std::string& String);
}
