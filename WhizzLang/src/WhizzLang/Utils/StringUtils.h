#pragma once

namespace WhizzLang {

	namespace StringUtils {

		inline size_t FindNthOf(const std::string& string, char ch, size_t n)
		{
			size_t length = string.size();

			for (int32_t i = 0; i < length; i++)
			{
				if (string[i] == ch)
					n--;

				if (n == 0)
					return i;
				if (n < 0)
					return -1;
			}

			return -1;
		}

	}

}
