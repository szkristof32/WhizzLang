#pragma once

#include "WhizzLang/Core/Tokeniser/Tokens.h"

#include "WhizzLang/Core/Parser/Nodes.h"

namespace WhizzLang {

	class Parser
	{
	public:
		Parser() = default;
		Parser(const std::string_view filename)
			: m_Filename(filename)
		{
		}
		~Parser();

		void Parse(std::span<Token> tokens);

		NodeProgram* GetProgram() const { return m_Program; };
	private:
		std::optional<Token> Peek(size_t count = 0);
		const Token& Consume();
		const Token& TryConsume(TokenType type);
		std::span<Token> TryConsume(std::initializer_list<TokenType> types);
		const Token& GetPreviousToken() const { return m_Tokens[std::max(m_Index - 1, 0ull)]; }
		const Token& GetCurrentToken() const { return m_Tokens[m_Index]; }

		NodeProgram* ParseProgram();
		NodeFunction* ParseFunction();
		NodeStatement* ParseStatement();
		NodeExpression* ParseExpression();
		NodeTerm* ParseTerm();
	private:
		std::span<Token> m_Tokens;
		std::string_view m_Filename;

		NodeProgram* m_Program = nullptr;
		size_t m_Index = 0;
	};

}
