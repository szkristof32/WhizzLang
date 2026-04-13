#pragma once

#include <utility>

namespace std {

#ifdef _MSC_VER
#define CPP_VERSION _MSVC_LANG
#else
#define CPP_VERSION __cplusplus
#endif

#if CPP_VERSION <= 201103L

	template<typename _Ty, typename _Other = _Ty>
	constexpr _Ty exchange(_Ty& value, _Other&& new_value) noexcept
	{
		_Ty temp = std::move(value);
		value = std::forward<_Other>(new_value);
		return temp;
	}

#endif

}
