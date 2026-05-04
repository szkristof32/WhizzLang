#pragma once

#include "WhizzLang/Errors/CompilerError.h"

#include <string_view.h>

namespace WhizzLang {

	class GeneratorError : public CompilerError
	{
	public:
		GeneratorError(const std::string& message, const std::string_view filename, size_t line, size_t column)
			: CompilerError(filename, line, column), m_Message(message)
		{
		}

		std::string_view GetMessage() const { return m_Message; }
	private:
		std::string m_Message;
	};

}
