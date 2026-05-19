#pragma once

#include "WhizzLang/Core/Tokeniser/Tokens.h"

#include "string_view.h"
#include "optional.h"

namespace WhizzLang {

	class Tokeniser
	{
	public:
		Tokeniser() = default;
		Tokeniser(const std::string_view source, const std::string_view filename);

		void Tokenise();

		std::span<Token> GetTokens() const { return std::span<Token>(m_Tokens.data(), m_Tokens.size()); }
	private:
		std::optional<char> Peek(size_t length = 0);
		char Consume();

		Token CreateToken() const;

		void TokeniseAlnum();
		void TokeniseNumeric();
		bool HandleComments();

		void NewLine();
	private:
		std::string_view m_Source;
		std::string_view m_Filename;
		std::vector<Token> m_Tokens;

		size_t m_Index = 0;
		size_t m_Line = 1, m_Column = 1;
	};

}
