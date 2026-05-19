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
					token.Type = TokenType::OpenBracket;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case ')':
				{
					Consume();
					token.Type = TokenType::CloseBracket;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '{':
				{
					Consume();
					token.Type = TokenType::OpenBrace;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '}':
				{
					Consume();
					token.Type = TokenType::CloseBrace;
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
				case '-':
				{
					Consume();
					token.Type = TokenType::Minus;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '*':
				{
					Consume();
					token.Type = TokenType::Star;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '/':
				{
					Consume();

					bool shouldTokenise = HandleComments();
					if (!shouldTokenise)
						continue;

					token.Type = TokenType::Slash;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '=':
				{
					Consume();
					token.Type = TokenType::Equal;
					m_Tokens.emplace_back(std::move(token));
					continue;
				}
				case '\n':
				{
					Consume();
					NewLine();
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
		if (buffer == "const")
		{
			token.Type = TokenType::KeywordConst;
			m_Tokens.emplace_back(std::move(token));
			return;
		}
		if (buffer == "if")
		{
			token.Type = TokenType::KeywordIf;
			m_Tokens.emplace_back(std::move(token));
			return;
		}
		if (buffer == "else")
		{
			token.Type = TokenType::KeywordElse;
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

	bool Tokeniser::HandleComments()
	{
		if (!Peek().has_value())
			return true;

		switch (Peek().value())
		{
			case '/':
			{
				Consume();

				while (Peek() && Peek().value() != '\n')
					Consume();
				Consume();
				NewLine();
				
				return false;
			}
			case '*':
			{
				Consume();

				while (Peek(1) && Peek().value() != '*' && Peek(1).value() != '/')
				{
					char c = Consume();
					if (c == '\n')
						NewLine();
				}
				Consume();
				Consume();
				return false;
			}
		}

		return true;
	}

	void Tokeniser::NewLine()
	{
		m_Line++;
		m_Column = 1;
	}

}
