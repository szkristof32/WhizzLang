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

#define metadata m_Filename, GetPreviousToken().Line, GetPreviousToken().Column

	NodeProgram* Parser::ParseProgram()
	{
		NodeProgram* program = new NodeProgram(metadata);

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
		TryConsume({ TokenType::OpenBracket, TokenType::CloseBracket });
		TryConsume(TokenType::Colon);
		auto& returnType = TryConsume(TokenType::KeywordInt);
		NodeFunction* fn = new NodeFunction(identifier, returnType, metadata);

		NodeScope* scope = ParseScope();
		fn->PushChild(scope);

		return fn;
	}

	NodeScope* Parser::ParseScope()
	{
		TryConsume(TokenType::OpenBrace);

		NodeScope* scope = new NodeScope(metadata);
		while (Peek().has_value() && Peek()->Type != TokenType::CloseBrace)
		{
			switch (Peek()->Type)
			{
				case TokenType::OpenBrace:
				{
					NodeScope* newScope = ParseScope();
					scope->PushChild(newScope);
					break;
				}
				default:
				{
					NodeStatement* statement = ParseStatement();
					scope->PushChild(statement);
					break;
				}
			}
		}

		TryConsume(TokenType::CloseBrace);

		return scope;
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
				NodeStatement* statement = new NodeReturn(metadata);
				statement->PushChild(expression);
				return statement;
			}
			case TokenType::KeywordConst:
			{
				Consume();
				const auto& name = TryConsume(TokenType::Identifier);
				TryConsume(TokenType::Equal);
				NodeExpression* expression = ParseExpression();
				TryConsume(TokenType::Semicolon);
				NodeStatement* statement = new NodeVariable(name, metadata);
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
					lhs = new NodeBinaryExpressionAdd(lhs, rhs, metadata);
					break;
				}
				case TokenType::Minus:
				{
					lhs = new NodeBinaryExpressionSubtract(lhs, rhs, metadata);
					break;
				}
				case TokenType::Star:
				{
					lhs = new NodeBinaryExpressionMultiply(lhs, rhs, metadata);
					break;
				}
				case TokenType::Slash:
				{
					lhs = new NodeBinaryExpressionDivide(lhs, rhs, metadata);
					break;
				}
			}
		}

		return lhs;
	}

	NodeTerm* Parser::ParseTerm()
	{
		auto token = Peek();

		if (!token.has_value())
			throw ParserError("No token left", std::nullopt, m_Filename, GetPreviousToken().Line, GetPreviousToken().Column);

		switch (token->Type)
		{
			case TokenType::IntegerLiteral:
			{
				auto& integerLiteral = Consume();
				NodeTerm* term = new NodeTermIntegerLiteral(integerLiteral, metadata);
				return term;
			}
			case TokenType::OpenBracket:
			{
				Consume();
				auto expression = ParseExpression();
				TryConsume(TokenType::CloseBracket);
				NodeTerm* term = new NodeTermBracket(expression, metadata);
				return term;
			}
			case TokenType::Identifier:
			{
				auto& identifier = Consume();
				NodeTerm* term = new NodeTermIdentifier(identifier, metadata);
				return term;
			}
		}

		throw ParserError("Unexpected token: Expected IntegerLiteral or OpenBraces", std::nullopt, m_Filename, token->Line, token->Column);
	}

}
