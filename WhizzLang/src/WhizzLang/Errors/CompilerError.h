#pragma once

namespace WhizzLang {

	class CompilerError
	{
	public:
		CompilerError(const std::string_view filename, size_t line, size_t column)
			: m_Filename(filename), m_Line(line), m_Column(column)
		{
		}

		std::string_view GetFilename() const { return m_Filename; }
		size_t GetLine() const { return m_Line; }
		size_t GetColumn() const { return m_Column; }
	protected:
		std::string_view m_Filename;
		size_t m_Line = 1;
		size_t m_Column = 1;
	};

}
