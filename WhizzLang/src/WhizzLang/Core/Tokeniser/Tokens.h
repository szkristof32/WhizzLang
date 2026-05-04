#pragma once

namespace WhizzLang {

	enum class TokenType
	{
		None = 0,
		KeywordFn, KeywordInt, KeywordReturn,
		Identifier,
		OpenBraces, CloseBraces, OpenBrackets, CloseBrackets,
		Colon, Semicolon,
		IntegerLiteral,
		Plus, Minus, Star, Slash
	};

	namespace TokenUtils {

		std::string_view TokenTypeToString(TokenType type);
		uint32_t BinaryPrecedence(TokenType type);

	}

	template<typename _Os>
	_Os& operator<<(_Os& os, const TokenType& tokenType)
	{
		os << TokenUtils::TokenTypeToString(tokenType).data();
		return os;
	}

	struct Token
	{
		TokenType Type;
		std::string Buffer;

		size_t Line;
		size_t Column;
	};

}
