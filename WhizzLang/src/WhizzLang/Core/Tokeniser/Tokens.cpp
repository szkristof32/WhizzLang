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
			case TokenType::KeywordConst:	return "KeywordConst";
			case TokenType::KeywordIf:		return "KeywordIf";
			case TokenType::KeywordElse:	return "KeywordElse";
			case TokenType::Identifier:		return "Identifier";
			case TokenType::OpenBracket:	return "OpenBracket";
			case TokenType::CloseBracket:	return "CloseBracket";
			case TokenType::OpenBrace:		return "OpenBrace";
			case TokenType::CloseBrace:		return "CloseBrace";
			case TokenType::Colon:			return "Colon";
			case TokenType::Semicolon:		return "Semicolon";
			case TokenType::IntegerLiteral:	return "IntegerLiteral";
			case TokenType::Plus:			return "Plus";
			case TokenType::Minus:			return "Minus";
			case TokenType::Star:			return "Star";
			case TokenType::Slash:			return "Slash";
			case TokenType::Equal:			return "Equal";
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
