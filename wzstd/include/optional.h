#pragma once

#include "utility.h"

#include <exception>
#include <cstdint>

namespace std {

	struct nullopt_t
	{
		constexpr explicit nullopt_t(int) {}
	};

	constexpr nullopt_t nullopt{ 0 };

	class bad_optional_access : public exception
	{
	public:
		virtual const char* what() const noexcept override { return "bad optional access"; }
	};

	template<typename _Ty>
	class optional
	{
	public:
		using value_type = _Ty;
	public:
		optional() = default;
		optional(std::nullopt_t) : m_Padding(0), m_HasValue(false) {}
		optional(const value_type& value) : m_Value(value), m_HasValue(true) {}
		optional(value_type&& value) : m_Value(value), m_HasValue(true) {}
		~optional() { destroy(); }

		optional(const optional& other) : m_Padding(0), m_HasValue(other.m_HasValue)
		{
			if (m_HasValue)
				m_Value = other.m_Value;
		}
		optional(optional&& other) : m_Padding(0), m_HasValue(other.m_HasValue)
		{
			if (m_HasValue)
				std::exchange(m_Value, other.m_Value);
		}

		optional& operator=(const optional& other)
		{
			if (other.m_HasValue)
			{
				if (m_HasValue)
					m_Value = other.m_Value;
				else
					new (&m_Value) value_type(other.m_Value);
			}
			else
			{
				destroy();
				m_Padding = 0;
			}

			m_HasValue = other.m_HasValue;

			return *this;
		}
		optional& operator=(optional&& other)
		{
			if (other.m_HasValue)
			{
				if (m_HasValue)
					std::exchange(m_Value, other.m_Value);
				else
					new (&m_Value) value_type(other.m_Value);
			}
			else
			{
				destroy();
				m_Padding = 0;
			}

			m_HasValue = other.m_HasValue;

			return *this;
		}
		optional& operator=(std::nullopt_t)
		{
			destroy();
			m_Padding = 0;
			m_HasValue = false;

			return *this;
		}

		value_type* operator->() { return &value(); }
		const value_type* operator->() const { return &value(); }
		value_type& operator*() { return value(); }
		const value_type& operator*() const { return value(); }

		operator bool() const { return has_value(); }
		bool has_value() const { return m_HasValue; }

		value_type& value()
		{
			if (has_value())
				return m_Value;
			throw bad_optional_access{};
		}
		const value_type& value() const
		{
			if (has_value())
				return m_Value;
			throw bad_optional_access{};
		}
	private:
		void destroy() const { if (m_HasValue) m_Value.~value_type(); }
	private:
		union
		{
			value_type m_Value;
			uint8_t m_Padding = 0;
		};
		bool m_HasValue = false;
	};

}
