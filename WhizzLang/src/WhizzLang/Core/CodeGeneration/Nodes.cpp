#include "pch.h"
#include "WhizzLang/Core/Parser/Nodes.h"

namespace WhizzLang {

	std::string NodeProgram::GenerateCode() const
	{
		std::stringstream ss;
		ss << "\tglobal _start\n";
		ss << "\tsection .text\n";
		ss << "_start:\n";
		ss << "\tcall main\n";
		ss << "\tmov rdi, rax\n";
		ss << "\tmov rax, 60\n";
		ss << "\tsyscall\n";

		for (const auto& child : m_Children)
		{
			ss << child->GenerateCode();
		}

		return ss.str();
	}

	std::string NodeFunction::GenerateCode() const
	{
		std::stringstream ss;
		ss << m_Identifier.Buffer << ":\n";
		
		for (const auto& child : m_Children)
		{
			ss << child->GenerateCode();
		}

		ss << "\tret\n";

		return ss.str();
	}

	std::string NodeReturn::GenerateCode() const
	{
		std::stringstream ss;

		for (const auto& child : m_Children)
		{
			ss << child->GenerateCode();
		}

		ss << "\tmov rax, r8\n";

		return ss.str();
	}

	std::string NodeExpression::GenerateCode() const
	{
		std::stringstream ss;
		ss << "\tmov r8, " << m_IntegerLiteral.Buffer << "\n";
		return ss.str();
	}

}
