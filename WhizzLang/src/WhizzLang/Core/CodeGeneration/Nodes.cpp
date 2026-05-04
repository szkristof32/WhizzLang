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

	std::string NodeTermIntegerLiteral::GenerateCode() const
	{
		std::stringstream ss;
		ss << "\tmov r8, " << m_IntegerLiteral.Buffer << "\n";
		return ss.str();
	}

	std::string NodeBinaryExpressionAdd::GenerateCode() const
	{
		std::stringstream ss;

		ss << m_Lhs->GenerateCode();
		ss << "\tpush r8\n";
		ss << m_Rhs->GenerateCode();
		ss << "\tpop r9\n";
		ss << "\tadd r8, r9\n";

		return ss.str();
	}

	std::string NodeBinaryExpressionSubtract::GenerateCode() const
	{
		std::stringstream ss;

		ss << m_Lhs->GenerateCode();
		ss << "\tpush r8\n";
		ss << m_Rhs->GenerateCode();
		ss << "\tmov r9, r8\n";
		ss << "\tpop r8\n";
		ss << "\tsub r8, r9\n";

		return ss.str();
	}

	std::string NodeBinaryExpressionMultiply::GenerateCode() const
	{
		std::stringstream ss;

		ss << m_Lhs->GenerateCode();
		ss << "\tpush r8\n";
		ss << m_Rhs->GenerateCode();
		ss << "\tpop rax\n";
		ss << "\timul rax, r8\n";
		ss << "\tmov r8, rax\n";

		return ss.str();
	}

	std::string NodeBinaryExpressionDivide::GenerateCode() const
	{
		std::stringstream ss;

		ss << m_Lhs->GenerateCode();
		ss << "\tpush r8\n";
		ss << m_Rhs->GenerateCode();
		ss << "\tpop rax\n";
		ss << "\tcdq\n";
		ss << "\tidiv r8\n";
		ss << "\tmov r8, rax\n";

		return ss.str();
	}

	std::string NodeTermBraces::GenerateCode() const
	{
		return m_Expression->GenerateCode();
	}

}
