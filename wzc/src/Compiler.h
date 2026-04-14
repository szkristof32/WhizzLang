#pragma once

#include "string_view.h"

namespace WhizzLang {

	struct CompilerConfiguration
	{
		std::string_view SourceFile;
		std::string_view OutputFile = "a.out";
	};

	class Compiler
	{
	public:
		Compiler() = default;
		Compiler(const CompilerConfiguration& config);

		void Process();
	private:
		CompilerConfiguration m_Configuration;
		std::string m_Source;
	};

}
