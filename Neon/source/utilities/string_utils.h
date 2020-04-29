#pragma once

#include <string>

namespace Neon
{
	inline std::string WStringToString(std::wstring _str) { return std::string(_str.begin(), _str.end()); }
	inline std::wstring StringToWString(std::string _str) { return std::wstring(_str.begin(), _str.end()); }
}