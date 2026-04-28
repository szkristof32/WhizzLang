#pragma once

#include "WhizzLang/Errors/CompilerError.h"

#include <string_view.h>

namespace WhizzLang {

	class ParserError : public CompilerError
	{
	public:
		ParserError(const std::string_view message, std::optional<Token> token, const std::string_view filename, size_t line, size_t column)
			: CompilerError(filename, line, column), m_Message(message), m_Token(token)
		{
		}

		std::string_view GetMessage() const { return m_Message; }
		std::optional<Token> GetToken() const { return m_Token; }
	private:
		std::string m_Message;
		std::optional<Token> m_Token;
	};

}
