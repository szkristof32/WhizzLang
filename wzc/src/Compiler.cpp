#include "pch.h"
#include "Compiler.h"

#include "FileUtils.h"

namespace WhizzLang {

	Compiler::Compiler(const CompilerConfiguration& config)
		: m_Configuration(config)
	{
		m_Source = FileUtils::ReadFile(config.SourceFile);
	}

	void Compiler::Process()
	{
	}

}
