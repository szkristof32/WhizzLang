#pragma once

#include "WhizzLang/Core/Tokeniser/Tokeniser.h"
#include "WhizzLang/Core/Parser/Parser.h"
#include "WhizzLang/Core/CodeGeneration/CodeGenerator.h"

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

		std::string_view GetAssembly() const { return m_Assembly; }
	private:
		void SplitSource();
	private:
		CompilerConfiguration m_Configuration;
		std::string m_Source;
		std::vector<std::string_view> m_Lines;

		Tokeniser m_Tokeniser;
		Parser m_Parser;
		CodeGenerator m_Generator;
		std::string m_Assembly;
	};

}
