#include "pch.h"
#include "WhizzLang/Core/Parser/Nodes.h"

#include "WhizzLang/Errors/GeneratorError.h"

namespace WhizzLang {

	void NodeProgram::GenerateCode(CodeGenerator& generator) const
	{
		generator << "\tglobal _start\n";
		generator << "\tsection .text\n";
		generator << "_start:\n";
		generator << "\tcall main\n";
		generator << "\tmov rdi, rax\n";
		generator << "\tmov rax, 60\n";
		generator << "\tsyscall\n";

		for (const auto& child : m_Children)
		{
			child->GenerateCode(generator);
		}
	}

	void NodeFunction::GenerateCode(CodeGenerator& generator) const
	{
		generator.StartFunction(m_Identifier.Buffer);

		generator << m_Identifier.Buffer << ":\n";
		
		for (const auto& child : m_Children)
		{
			child->GenerateCode(generator);
		}

		generator.EndFunction();
	}

	void NodeReturn::GenerateCode(CodeGenerator& generator) const
	{
		for (const auto& child : m_Children)
		{
			child->GenerateCode(generator);
		}

		generator << "\tmov rax, r8\n";
		generator.EndFunction();
		generator << "\tret\n";
	}

	void NodeVariable::GenerateCode(CodeGenerator& generator) const
	{
		if (generator.FindVariable(m_Identifier.Buffer).has_value())
			throw GeneratorError("Variable already defined", m_Filename, m_Line, m_Column);
		generator.PushVariable(m_Identifier.Buffer);

		for (const auto& child : m_Children)
		{
			child->GenerateCode(generator);
		}

		generator.Push("r8");
	}

	void NodeTermIntegerLiteral::GenerateCode(CodeGenerator& generator) const
	{
		generator << "\tmov r8, " << m_IntegerLiteral.Buffer << "\n";
	}

	void NodeBinaryExpressionAdd::GenerateCode(CodeGenerator& generator) const
	{
		m_Lhs->GenerateCode(generator);
		generator.Push("r8");
		m_Rhs->GenerateCode(generator);
		generator.Pop("r9");
		generator << "\tadd r8, r9\n";
	}

	void NodeBinaryExpressionSubtract::GenerateCode(CodeGenerator& generator) const
	{
		m_Lhs->GenerateCode(generator);
		generator.Push("r8");
		m_Rhs->GenerateCode(generator);
		generator << "\tmov r9, r8\n";
		generator.Pop("r8");
		generator << "\tsub r8, r9\n";
	}

	void NodeBinaryExpressionMultiply::GenerateCode(CodeGenerator& generator) const
	{
		m_Lhs->GenerateCode(generator);
		generator.Push("r8");
		m_Rhs->GenerateCode(generator);
		generator.Pop("r9");
		generator << "\timul r8, r9\n";
	}

	void NodeBinaryExpressionDivide::GenerateCode(CodeGenerator& generator) const
	{
		m_Lhs->GenerateCode(generator);
		generator.Push("r8");
		m_Rhs->GenerateCode(generator);
		generator.Pop("rax");
		generator << "\tcdq\n";
		generator << "\tidiv r8\n";
		generator << "\tmov r8, rax\n";
	}

	void NodeTermBracket::GenerateCode(CodeGenerator& generator) const
	{
		return m_Expression->GenerateCode(generator);
	}

	void NodeTermIdentifier::GenerateCode(CodeGenerator& generator) const
	{
		auto variable = generator.FindVariable(m_Identifier.Buffer);
		if (!variable.has_value())
			throw GeneratorError(fmt::format("Undeclared identifier `{}`", m_Identifier.Buffer), m_Filename, m_Line, m_Column);
		generator << "\tmov r8, QWORD [rsp + " << (generator.GetStackSize() - variable->StackLocation - 1) * 8 << "]\n";
	}

	void NodeScope::GenerateCode(CodeGenerator& generator) const
	{
		size_t currentScope = generator.PushScope();

		for (const auto& child : m_Children)
		{
			child->GenerateCode(generator);
		}

		generator.PopScope(currentScope);
	}

	void NodeIf::GenerateCode(CodeGenerator& generator) const
	{
		Node* predicate = m_Children[0];
		Node* scope = m_Children[1];

		size_t labelIndex = generator.NewLabel();
		auto label = fmt::format(".LB{:03}", labelIndex);

		predicate->GenerateCode(generator);

		generator << "\tcmp r8, 0\n";
		generator << "\tje " << label << "\n";
		generator.BeginIf();

		scope->GenerateCode(generator);

		generator << label << ":\n";
		generator.Pop();
	}

}
