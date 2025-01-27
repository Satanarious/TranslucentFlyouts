#include "pch.h"
#include "RegHelper.hpp"

using namespace std;
using namespace wil;
using namespace::TranslucentFlyouts;

wstring_view g_regPath{L"Software\\TranslucentFlyouts"};

DWORD RegHelper::GetDword(std::wstring_view subItemName, std::wstring_view valueName, DWORD defaultValue, bool useFallback)
{
	DWORD regValue{defaultValue};

	[&]()
	{
		HRESULT hr{S_OK};
		if (subItemName.empty() && !useFallback)
		{
			return;
		}

		if (!subItemName.empty())
		{
			auto subKeyName
			{
				format(L"{}\\{}", g_regPath, subItemName)
			};
			hr = reg::get_value_dword_nothrow(
					 HKEY_CURRENT_USER, subKeyName.c_str(), valueName.data(), &regValue
				 );

			if (FAILED(hr))
			{
				hr = reg::get_value_dword_nothrow(
						 HKEY_LOCAL_MACHINE, subKeyName.c_str(), valueName.data(), &regValue
					 );
			}
			LOG_HR_IF(hr, FAILED(hr) && hr != HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND));

			if (!useFallback || SUCCEEDED(hr))
			{
				return;
			}
		}

		hr = reg::get_value_dword_nothrow(
				 HKEY_CURRENT_USER, g_regPath.data(), valueName.data(), &regValue
			 );
		if (FAILED(hr))
		{
			reg::get_value_dword_nothrow(
				HKEY_LOCAL_MACHINE, g_regPath.data(), valueName.data(), &regValue
			);
		}

		LOG_HR_IF(hr, FAILED(hr) && hr != HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND));
	}();

	return regValue;
}