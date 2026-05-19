#include "pch.h"
#include "Compiler.h"

#include "WhizzLang/Errors/SyntaxError.h"
#include "WhizzLang/Errors/ParserError.h"
#include "WhizzLang/Errors/GeneratorError.h"

#include "WhizzLang/Utils/FileUtils.h"
#include "WhizzLang/Utils/StringUtils.h"

namespace WhizzLang {

	Compiler::Compiler(const CompilerConfiguration& config)
		: m_Configuration(config)
	{
		m_Source = FileUtils::ReadFile(config.SourceFile);
		SplitSource();

		m_Tokeniser = Tokeniser(m_Source, config.SourceFile);
		m_Parser = Parser(config.SourceFile);
	}

	void Compiler::Process()
	{
		try
		{
			m_Tokeniser.Tokenise();
		}
		catch (SyntaxError error)
		{
			PrintErrorMessage(error, error.GetMessage());
			return;
		}

		try
		{
			m_Parser.Parse(m_Tokeniser.GetTokens());
		}
		catch (ParserError error)
		{
			PrintErrorMessage(error, error.GetMessage());
			return;
		}

		try
		{
			m_Parser.GetProgram()->GenerateCode(m_Generator);
			m_Assembly = m_Generator.GetCode().str();
		}
		catch (GeneratorError error)
		{
			PrintErrorMessage(error, error.GetMessage());
			return;
		}
	}

	void Compiler::SplitSource()
	{
		size_t offset = 0;

		while (offset != m_Source.size())
		{
			size_t eof = m_Source.find_first_of('\n', offset);
			m_Lines.emplace_back(std::string_view(&m_Source[offset], eof - offset));
			offset = eof + 1;
		}
	}

	void Compiler::PrintErrorMessage(CompilerError error, const std::string_view message)
	{
		size_t line = error.GetLine();
		size_t column = error.GetColumn();
		auto lineStr = fmt::format("{}", line);

		fmt::println(stderr, "Syntax error in {} at {}:{}: {}", error.GetFilename(), line, column, message);
		fmt::println(stderr, " {} | {}", lineStr, m_Lines[line - 1]);
		fmt::println(stderr, " {: >{}} | {: >{}}", "", lineStr.size(), "^", column);
	}

}
