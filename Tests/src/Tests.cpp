#include "optional.h"
#include "string_view.h"
#include "filesystem.h"

#include <pch.h>

#include "WhizzLang/Core/Tokeniser/Tokeniser.h"

#include <memtrace.h>
#include <gtest_lite.h>

#include <fmt/format.h>

struct TestStruct
{
	uint32_t Value = 64;
};

int main(int argc, char** argv)
{
#pragma region wzstd
	fmt::println("wzstd tests...");

	fmt::println("\n== optional ==");

	TEST(optional, EmptyConstruct)
	{
		std::optional<uint32_t> opt;
		EXPECT_FALSE(opt.has_value());
		EXPECT_THROW(opt.value(), std::bad_optional_access);
	}
	END;

	TEST(optional, NulloptConstruct)
	{
		std::optional<uint32_t> opt = std::nullopt;
		EXPECT_FALSE(opt.has_value());
		EXPECT_THROW(opt.value(), std::bad_optional_access);
	}
	END;

	TEST(optional, Ctor)
	{
		std::optional<uint32_t> opt = 32;
		EXPECT_TRUE(opt.has_value());
		EXPECT_NO_THROW(opt.value());
		EXPECT_EQ(32, opt.value());
	}
	END;

	TEST(optional, Copy)
	{
		std::optional<uint32_t> opt1 = 32;
		std::optional<uint32_t> opt2 = opt1;
		std::optional<uint32_t> opt3 = 8;
		opt3 = opt2;
		EXPECT_EQ(32, opt2.value());
		EXPECT_EQ(32, opt3.value());
	}
	END;

	TEST(optional, Move)
	{
		std::optional<uint32_t> opt1 = 32;
		std::optional<uint32_t> opt2 = std::move(opt1);
		EXPECT_EQ(32, opt2.value());
	}
	END;

	TEST(optional, Operators)
	{
		std::optional<uint32_t> opt1 = 32;
		std::optional<TestStruct> opt2 = TestStruct{};
		EXPECT_EQ(32, opt1.value());
		EXPECT_EQ(32, *opt1);
		EXPECT_EQ(64, opt2->Value);
		EXPECT_TRUE(opt2);
	}
	END;

	fmt::println("\n== string_view ==");

	TEST(string_view, EmptyConstruct)
	{
		std::string_view sv;
		EXPECT_EQ(0ull, sv.size());
		EXPECT_EQ((const char*)nullptr, sv.data());
	}
	END;

	TEST(string_view, Ctor)
	{
		using namespace std::literals::string_view_literals;

		std::string_view sv1 = "Hello";
		auto sv2 = "Hi"sv;
		EXPECT_EQ(5ull, sv1.size());
		EXPECT_EQ('l', sv1.at(3));
		EXPECT_EQ(2ull, sv2.size());
		EXPECT_EQ('i', sv2[1]);
	}
	END;

	TEST(string_view, Copy)
	{
		std::string_view sv1("Hello", 3);
		std::string_view sv2 = sv1;
		std::string_view sv3 = "Hello";
		sv3 = sv2;
		EXPECT_EQ(3ull, sv1.size());
		EXPECT_EQ(3ull, sv2.size());
		EXPECT_EQ(sv1.data(), sv2.data());
		EXPECT_EQ(3ull, sv3.size());
	}
	END;

	fmt::println("\n== filesystem ==");

	TEST(filesystem, path)
	{
		std::filesystem::path p = "src/Tests.cpp";
		EXPECT_EQ(13ull, p.size());
		EXPECT_EQ('/', p.at(3));
	}
	END;

	TEST(filesystem, exists)
	{
		std::filesystem::path p1 = "src/Tests.cpp";
		std::filesystem::path p2 = "src/Tests_don't exists.cpp";
		EXPECT_TRUE(std::filesystem::exists(p1));
		EXPECT_FALSE(std::filesystem::exists(p2));
	}
	END;
#pragma endregion

	fmt::println("\nwzc tests...");

	fmt::println("\n== Tokeniser ==");

	TEST(Tokeniser, Tokenise1)
	{
		WhizzLang::Tokeniser tokeniser("return 0;", "Tokeniser.Tokenise1"); tokeniser.Tokenise();
		const auto& tokens = tokeniser.GetTokens();
		EXPECT_EQ(3ull, tokens.size());
		EXPECT_EQ(WhizzLang::TokenType::KeywordReturn, tokens[0].Type);
		EXPECT_EQ(WhizzLang::TokenType::IntegerLiteral, tokens[1].Type);
		EXPECT_EQ(8ull, tokens[1].Column);
		EXPECT_EQ(WhizzLang::TokenType::Semicolon, tokens[2].Type);
	}
	END;

	TEST(Tokeniser, Tokenise2)
	{
		WhizzLang::Tokeniser tokeniser("fn main() : int\n{}", "Tokeniser.Tokenise2"); tokeniser.Tokenise();
		const auto& tokens = tokeniser.GetTokens();
		EXPECT_EQ(8ull, tokens.size());
		EXPECT_EQ(WhizzLang::TokenType::KeywordFn, tokens[0].Type);
		EXPECT_EQ(WhizzLang::TokenType::Identifier, tokens[1].Type);
		EXPECT_EQ(WhizzLang::TokenType::OpenBracket, tokens[2].Type);
		EXPECT_EQ(WhizzLang::TokenType::CloseBracket, tokens[3].Type);
		EXPECT_EQ(WhizzLang::TokenType::Colon, tokens[4].Type);
		EXPECT_EQ(11ull, tokens[4].Column);
		EXPECT_EQ(WhizzLang::TokenType::KeywordInt, tokens[5].Type);
		EXPECT_EQ(WhizzLang::TokenType::OpenBrace, tokens[6].Type);
		EXPECT_EQ(WhizzLang::TokenType::CloseBracket, tokens[7].Type);
		EXPECT_EQ(2ull, tokens[6].Line);
		EXPECT_EQ(2ull, tokens[7].Line);
		EXPECT_EQ(2ull, tokens[7].Column);
	}
	END;
}
