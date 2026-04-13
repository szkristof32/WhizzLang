#include "filesystem.h"

#ifdef WL_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(WL_PLATFORM_LINUX)
#include <sys/stat.h>
#endif

namespace std {

	namespace filesystem {

		bool _exists_impl(const path& path)
		{
#ifdef WL_PLATFORM_WINDOWS
			DWORD attributes = GetFileAttributesA(path.c_str());
			return attributes != INVALID_FILE_ATTRIBUTES;
#elif defined(WL_PLATFORM_LINUX)
			struct stat buffer;
			return stat(path.c_str(), &buffer) == 0;
#endif
		}

	}

}
