#include "pch.h"
#include "Compiler.h"

#include "WhizzLang/Utils/FileUtils.h"

namespace WhizzLang {

	Compiler::Compiler(const CompilerConfiguration& config)
		: m_Configuration(config)
	{
		m_Source = FileUtils::ReadFile(config.SourceFile);
		m_Tokeniser = Tokeniser(m_Source);
	}

	void Compiler::Process()
	{
		m_Tokeniser.Tokenise();
		m_Parser.Parse(m_Tokeniser.GetTokens());
	}

}
