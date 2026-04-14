#pragma once

#include <cstring>
#include <string>

#include "filesystem.h"

namespace std {

	template<typename _CharT>
	class basic_string_view
	{
	public:
		using value_type = _CharT;
		using pointer = _CharT*;
		using const_pointer = const _CharT*;
		using reference = _CharT&;
		using const_reference = const _CharT&;
		using size_type = size_t;
	public:
		basic_string_view() = default;
		basic_string_view(const_pointer buffer) : m_Buffer(buffer), m_Size(strlen(buffer)) {}
		basic_string_view(const_pointer buffer, size_type size) : m_Buffer(buffer), m_Size(size) {}
		basic_string_view(const std::string& string) : m_Buffer(string.data()), m_Size(string.size()) {}
		~basic_string_view() = default;

		basic_string_view(const basic_string_view& other) : m_Buffer(other.m_Buffer), m_Size(other.m_Size) {}
		basic_string_view(basic_string_view&& other) : m_Buffer(other.m_Buffer), m_Size(other.m_Size) {}

		basic_string_view& operator=(const basic_string_view& other)
		{
			m_Buffer = other.m_Buffer;
			m_Size = other.m_Size;

			return *this;
		}
		basic_string_view& operator=(basic_string_view&& other)
		{
			m_Buffer = other.m_Buffer;
			m_Size = other.m_Size;

			return *this;
		}

		const_pointer begin() { return m_Buffer; }
		const_pointer begin() const { return m_Buffer; }
		const_pointer end() { return m_Buffer + m_Size; }
		const_pointer end() const { return m_Buffer + m_Size; }

		const_reference operator[](size_type index) { return m_Buffer[index]; }
		const_reference at(size_type index) { return m_Buffer[index]; }

		const_reference front() const { return *m_Buffer; }
		const_reference back() const { return *(m_Buffer + m_Size - 1); }

		const_pointer data() const { return m_Buffer; }

		size_type size() const { return m_Size; }
		size_type length() const { return m_Size; }
		bool empty() const { return m_Size == 0; }

		template<typename _Os>
		_Os& operator<<(_Os& ostream)
		{
			ostream << m_Buffer;
			return ostream;
		}

		template<typename _Os>
		_Os& operator<<(_Os& ostream) const
		{
			ostream << m_Buffer;
			return ostream;
		}

		operator basic_string<value_type>() { return basic_string<value_type>(m_Buffer, m_Size); }
		operator basic_string<value_type>() const { return basic_string<value_type>(m_Buffer, m_Size); }
	private:
		const_pointer m_Buffer = nullptr;
		size_type m_Size = 0;
	};

	using string_view = basic_string_view<char>;

	template<>
	struct hash<string_view>
	{
		size_t operator()(const string_view& sv)
		{
			return hash()(std::string(sv.data(), sv.size()));
		}
	};

	namespace literals {

		namespace string_view_literals {

			inline string_view operator""sv(const char* string, size_t length)
			{
				return string_view(string, length);
			}

		}

	}

}
