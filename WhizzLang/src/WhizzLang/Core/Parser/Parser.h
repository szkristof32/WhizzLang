#pragma once

#include "WhizzLang/Core/Tokeniser/Tokens.h"

#include "WhizzLang/Core/Parser/Nodes.h"

namespace WhizzLang {

	class Parser
	{
	public:
		Parser() = default;
		~Parser();

		void Parse(std::span<Token> tokens);

		NodeProgram* GetProgram() const { return m_Program; };
	private:
		std::optional<Token> Peek(size_t count = 0);
		const Token& Consume();
		const Token& TryConsume(TokenType type);
		std::span<Token> TryConsume(std::initializer_list<TokenType> types);

		NodeProgram* ParseProgram();
		NodeFunction* ParseFunction();
		NodeStatement* ParseStatement();
		NodeExpression* ParseExpression();
	private:
		std::span<Token> m_Tokens;

		NodeProgram* m_Program;
		size_t m_Index = 0;
	};

}
