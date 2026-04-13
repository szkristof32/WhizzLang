#pragma once

#include <string>

namespace std {

	namespace filesystem {

		using path = std::string;

		bool _exists_impl(const path& path);

		inline bool exists(const path& path) { return _exists_impl(path); }

	}

}
