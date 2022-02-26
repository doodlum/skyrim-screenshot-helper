#pragma once

namespace WStringUtil
{
	// Chris Ohk @utilForever
	// https://gist.github.com/utilForever/fdf1540cea0de65cfc0a1a69d8cafb63#file-replacewcswithpattern-cpp

	std::wstring ReplaceWCSWithPattern(__in const std::wstring& message, __in const std::wstring& pattern, __in const std::wstring& replace)
	{
		std::wstring            result = message;
		std::wstring::size_type pos = 0;
		std::wstring::size_type offset = 0;

		while ((pos = result.find(pattern, offset)) != std::wstring::npos) {
			result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
			offset = pos + replace.size();
		}

		return result;
	}


	// Ryan @Ryan-rsm-McKenzie
	// https://github.com/Ryan-rsm-McKenzie/ScaleformTranslationPP/blob/660b623e1f4171d42daa39ca97a881689983ccb1/src/LocaleManager.cpp

	std::wstring ConvertStringToWString(const std::string& a_str)
	{
		if (a_str.empty()) {
			return std::wstring();
		}

		auto size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, a_str.c_str(), static_cast<int>(a_str.length()), nullptr, 0);
		bool err = size == 0;
		if (!err) {
			std::wstring strTo;
			strTo.resize(size);
			err = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, a_str.c_str(), static_cast<int>(a_str.length()), strTo.data(), size) == 0;
			if (!err) {
				return strTo;
			}
		}

		if (err) {
			logger::error("MultiByteToWideChar failed with error code ({})", GetLastError());
		}

		return std::wstring();
	}

	std::string ConvertWStringToString(const std::wstring& a_str)
	{
		if (a_str.empty()) {
			return std::string();
		}

		auto size = WideCharToMultiByte(CP_UTF8, 0, a_str.c_str(), static_cast<int>(a_str.length()), nullptr, 0, nullptr, nullptr);
		bool err = size == 0;
		if (!err) {
			std::string strTo;
			strTo.resize(size);
			err = WideCharToMultiByte(CP_UTF8, 0, a_str.c_str(), static_cast<int>(a_str.length()), strTo.data(), size, nullptr, nullptr) == 0;
			if (!err) {
				return strTo;
			}
		}

		if (err) {
			logger::error("WideCharToMultiByte failed with error code ({})", GetLastError());
		}

		return std::string();
	}

}
