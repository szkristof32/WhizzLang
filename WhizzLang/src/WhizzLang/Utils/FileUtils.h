#pragma once

#include "filesystem.h"

#include <fstream>

namespace WhizzLang {

	namespace FileUtils {

		template<typename _Buffer = std::string>
		inline _Buffer ReadFile(const std::filesystem::path& filepath)
		{
			std::ifstream file(filepath.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
			if (!file)
				throw std::runtime_error("Failed to open file!");

			size_t fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			_Buffer buffer;
			buffer.resize(fileSize / sizeof(typename _Buffer::value_type));
			file.read((char*)buffer.data(), fileSize);

			return buffer;
		}

		template<typename _Buffer = std::string>
		inline void WriteFile(const std::filesystem::path& filepath, const _Buffer& buffer)
		{
			std::ofstream file(filepath.c_str(), std::ios::out | std::ios::binary);
			if (!file)
				throw std::runtime_error("Failed to open file!");

			file.write((char*)buffer.data(), buffer.size() * sizeof(typename _Buffer::value_type));
		}

	}

}
