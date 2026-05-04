#include "pch.h"
#include "Tokens.h"

namespace WhizzLang {

	std::string_view TokenUtils::TokenTypeToString(TokenType type)
	{
		switch (type)
		{
			case TokenType::KeywordFn:		return "KeywordFn";
			case TokenType::KeywordInt:		return "KeywordInt";
			case TokenType::KeywordReturn:	return "KeywordReturn";
			case TokenType::Identifier:		return "Identifier";
			case TokenType::OpenBracket:		return "OpenBraces";
			case TokenType::CloseBracket:	return "CloseBraces";
			case TokenType::OpenBrace:	return "OpenBrackets";
			case TokenType::CloseBrace:	return "CloseBrackets";
			case TokenType::Colon:			return "Colon";
			case TokenType::Semicolon:		return "Semicolon";
			case TokenType::IntegerLiteral:	return "IntegerLiteral";
		}

		throw std::runtime_error("Invalid TokenType!");
	}

	uint32_t TokenUtils::BinaryPrecedence(TokenType type)
	{
		switch (type)
		{
			case TokenType::Plus:	return 1;
			case TokenType::Minus:	return 1;
			case TokenType::Star:	return 2;
			case TokenType::Slash:	return 2;
		}

		return 0;
	}

}
