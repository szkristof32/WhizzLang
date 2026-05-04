#include "pch.h"
#include "Parser.h"

#include "WhizzLang/Errors/ParserError.h"

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
		if (!Peek().has_value())
		{
			const auto& currentToken = GetPreviousToken();
			throw ParserError("No token left", std::nullopt, m_Filename, currentToken.Line, currentToken.Column);
		}

		if (Peek().value().Type == type)
			return Consume();

		throw ParserError(fmt::format("Unexpected token: Expected {}", TokenUtils::TokenTypeToString(type)),
			std::nullopt, m_Filename, Peek().value().Line, Peek().value().Column);
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
		{
			const auto& currentToken = GetPreviousToken();
			throw ParserError("No token left: Expected Statement", std::nullopt, m_Filename, currentToken.Line, currentToken.Column);
		}

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

		const auto& currentToken = GetCurrentToken();
		throw ParserError("Invalid token: Unexpected Statement", std::nullopt, m_Filename, currentToken.Line, currentToken.Column);
	}

	NodeExpression* Parser::ParseExpression(uint32_t minPrecedence)
	{
		NodeExpression* lhs = ParseTerm();

		while (Peek().has_value())
		{
			uint32_t precedence = TokenUtils::BinaryPrecedence(Peek()->Type);
			if (precedence == 0 || precedence < minPrecedence)
				break;

			Token op = Consume();
			uint32_t nextPrecedence = precedence + 1;
			NodeExpression* rhs = ParseExpression(nextPrecedence);

			switch (op.Type)
			{
				case TokenType::Plus:
				{
					lhs = new NodeBinaryExpressionAdd(lhs, rhs);
					break;
				}
				case TokenType::Minus:
				{
					lhs = new NodeBinaryExpressionSubtract(lhs, rhs);
					break;
				}
				case TokenType::Star:
				{
					lhs = new NodeBinaryExpressionMultiply(lhs, rhs);
					break;
				}
				case TokenType::Slash:
				{
					lhs = new NodeBinaryExpressionDivide(lhs, rhs);
					break;
				}
			}
		}

		return lhs;
	}

	NodeTerm* Parser::ParseTerm()
	{
		auto& integerLiteral = TryConsume(TokenType::IntegerLiteral);
		NodeTerm* term = new NodeTermIntegerLiteral(integerLiteral);
		return term;
	}

}
