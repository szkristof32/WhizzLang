#pragma once

#include <limits>

namespace std {

	constexpr size_t dynamic_extent = std::numeric_limits<size_t>::max();

	template<typename _Ty, size_t _Extent = dynamic_extent>
	class span
	{
	public:
		using element_type = _Ty;
		using value_type = remove_cv_t<_Ty>;
		using size_type = size_t;
		using pointer = _Ty*;
		using const_pointer = const _Ty*;
		using reference = _Ty&;
		using const_reference = const _Ty&;
	public:
		constexpr span() noexcept = default;
		constexpr span(const_pointer first, size_type count) : m_Data(first), m_Size(count) {}
		constexpr span(const_pointer front, pointer back) : m_Data(front), m_Size(back - front) {}
		template<typename _U, size_t _N>
		constexpr span(const std::array<_U, _N>& array) noexcept : m_Data((const_pointer)array.data()), m_Size(_N) {}
		~span() = default;

		span(const span& other) = default;
		span(span&& other) = default;

		span& operator=(const span& other)
		{
			m_Data = other.m_Data;
			m_Size = other.m_Size;
			return *this;
		}
		span& operator=(span&& other)
		{
			m_Data = other.m_Data;
			m_Size = other.m_Size;
			return *this;
		}

		pointer begin() { return m_Data; }
		const_pointer begin() const { return m_Data; }
		pointer end() { return m_Data + m_Size; }
		const_pointer end() const { return m_Data + m_Size; }

		reference front() { return *m_Data; }
		const_reference front() const { return *m_Data; }
		reference back() { return *(m_Data + m_Size - 1); }
		const_reference back() const { return *(m_Data + m_Size - 1); }

		const_reference at(size_t index) const { return m_Data[index]; }
		const_reference operator[](size_t index) const { return at(index); }

		const_pointer data() const { return m_Data; }

		size_type size() const { return m_Size; }
		bool empty() const { return m_Size == 0; }
	private:
		const_pointer m_Data = nullptr;
		size_type m_Size = 0;
	};

}
