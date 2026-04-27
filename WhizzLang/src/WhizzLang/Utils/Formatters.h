#pragma once

namespace fmt {

	template<>
	struct formatter<std::string_view>
	{
		template<typename _Ctx>
		constexpr auto parse(const _Ctx& ctx)
		{
			return ctx.begin();
		}

		template<typename _Ctx>
		constexpr auto format(const std::string_view& str, _Ctx& ctx) const
		{
			return format_to(ctx.out(), "{:.{}}", str.data(), str.size());
		}
	};

}

