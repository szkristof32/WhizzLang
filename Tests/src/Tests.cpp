#include "optional.h"

#include <pch.h>

#include <memtrace.h>
#include <gtest_lite.h>

#include <fmt/format.h>

struct TestStruct
{
	uint32_t Value = 64;
};

int main(int argc, char** argv)
{
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
}
