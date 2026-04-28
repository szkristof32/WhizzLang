#include "pch.h"
#include "Tokeniser.h"

#include "WhizzLang/Errors/SyntaxError.h"

namespace WhizzLang {

	Tokeniser::Tokeniser(const std::string_view source, const std::string_view filename)
		: m_Source(source), m_Filename(filename)
	{
	}

	void Tokeniser::Tokenise()
	{
		while (Peek().has_value())
		{
			char c = Peek().value();

			if (std::isalpha(c))
			{
				TokeniseAlnum();
				continue;
			}
			if (std::isdigit(c))
			{
				TokeniseNumeric();
				continue;
			}
			Token token = CreateToken();
			switch (c)
			{
				case ':':
				{
					Consume();
					token.Type = TokenType::Colon;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case ';':
				{
					Consume();
					token.Type = TokenType::Semicolon;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '(':
				{
					Consume();
					token.Type = TokenType::OpenBraces;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case ')':
				{
					Consume();
					token.Type = TokenType::CloseBraces;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '{':
				{
					Consume();
					token.Type = TokenType::OpenBrackets;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '}':
				{
					Consume();
					token.Type = TokenType::CloseBrackets;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '+':
				{
					Consume();
					token.Type = TokenType::Plus;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '\n':
				{
					m_Line++;
					m_Column = 0;
					Consume();
					continue;
				}
			}
			if (std::isspace(c))
			{
				Consume();
				continue;
			}

			throw SyntaxError("Unexpected character", m_Filename, m_Line, m_Column);
		}
	}

	std::optional<char> Tokeniser::Peek(size_t length)
	{
		if (m_Index + length < m_Source.size())
			return m_Source.at(m_Index + length);
		return std::nullopt;
	}

	char Tokeniser::Consume()
	{
		m_Column++;
		return m_Source[m_Index++];
	}

	Token Tokeniser::CreateToken() const
	{
		Token token{};
		token.Line = m_Line;
		token.Column = m_Column;

		return token;
	}

	void Tokeniser::TokeniseAlnum()
	{
		Token token = CreateToken();
		std::string buffer(1, Consume());

		while (Peek().has_value() && std::isalnum(Peek().value()))
		{
			buffer += Consume();
		}

		if (buffer == "fn")
		{
			token.Type = TokenType::KeywordFn;
			m_Tokens.emplace_back(std::move(token));
			return;
		}
		if (buffer == "int")
		{
			token.Type = TokenType::KeywordInt;
			m_Tokens.emplace_back(std::move(token));
			return;
		}
		if (buffer == "return")
		{
			token.Type = TokenType::KeywordReturn;
			m_Tokens.emplace_back(std::move(token));
			return;
		}

		token.Type = TokenType::Identifier;
		token.Buffer = buffer;
		m_Tokens.emplace_back(std::move(token));
	}

	void Tokeniser::TokeniseNumeric()
	{
		Token token = CreateToken();
		std::string buffer(1, Consume());

		while (Peek().has_value() && std::isdigit(Peek().value()))
		{
			buffer += Consume();
		}

		token.Type = TokenType::IntegerLiteral;
		token.Buffer = buffer;
		m_Tokens.emplace_back(std::move(token));
	}

}
