#include "pch.h"
#include "Parser.h"

namespace WhizzLang {

	Parser::~Parser()
	{
		delete m_Program;
	}

	void Parser::Parse(std::span<Token> tokens)
	{
		m_Tokens = tokens;

		m_Program = ParseProgram();
	}

	std::optional<Token> Parser::Peek(size_t count)
	{
		if (m_Index + count < m_Tokens.size())
			return m_Tokens[m_Index + count];
		return std::nullopt;
	}

	const Token& Parser::Consume()
	{
		return m_Tokens[m_Index++];
	}

	const Token& Parser::TryConsume(TokenType type)
	{
		if (Peek().has_value() && Peek().value().Type == type)
			return Consume();
		throw std::runtime_error("Unexpected token");
	}

	std::span<Token> Parser::TryConsume(std::initializer_list<TokenType> types)
	{
		for (const auto& type : types)
		{
			TryConsume(type);
		}
		return std::span<Token>(m_Tokens.data() + m_Index - types.size(), types.size());
	}

	NodeProgram* Parser::ParseProgram()
	{
		NodeProgram* program = new NodeProgram();

		while (Peek().has_value())
		{
			auto token = Peek().value();

			switch (token.Type)
			{
				case TokenType::KeywordFn:
				{
					auto fn = ParseFunction();
					program->PushChild(fn);
					break;
				}
			}
		}

		return program;
	}

	NodeFunction* Parser::ParseFunction()
	{
		Consume();
		auto& identifier = TryConsume(TokenType::Identifier);
		TryConsume({ TokenType::OpenBraces, TokenType::CloseBraces });
		TryConsume(TokenType::Colon);
		auto& returnType = TryConsume(TokenType::KeywordInt);
		NodeFunction* fn = new NodeFunction(identifier, returnType);

		TryConsume(TokenType::OpenBrackets);

		while (Peek().has_value() && Peek().value().Type != TokenType::CloseBrackets)
		{
			NodeStatement* statement = ParseStatement();
			fn->PushChild(statement);
		}

		TryConsume(TokenType::CloseBrackets);

		return fn;
	}

	NodeStatement* Parser::ParseStatement()
	{
		if (!Peek().has_value())
			throw std::runtime_error("No token left!");

		auto token = Peek().value();

		switch (token.Type)
		{
			case TokenType::KeywordReturn:
			{
				Consume();
				NodeExpression* expression = ParseExpression();
				TryConsume(TokenType::Semicolon);
				NodeReturn* statement = new NodeReturn();
				statement->PushChild(expression);
				return statement;
			}
		}

		throw std::runtime_error("Invalid statement!");
	}

	NodeExpression* Parser::ParseExpression()
	{
		const auto& integer = TryConsume(TokenType::IntegerLiteral);
		NodeExpression* expression = new NodeExpression(integer);
		return expression;
	}

}
