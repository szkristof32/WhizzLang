#include "pch.h"
#include "CodeGenerator.h"

namespace WhizzLang {

#if 0

	size_t CodeGenerator::OpenFunction()
	{
		Function function{};
		if (!m_Functions.empty())
		{
			const auto& lastFunction = GetCurrentFunction();
			function.StackLocation = lastFunction.StackLocation + lastFunction.FunctionSize;
		}
		m_Functions.emplace_back(std::move(function));

		m_Code << "; start of function no. " << m_Functions.size() - 1 << "\n";

		return m_Functions.size() - 1;
	}

	void CodeGenerator::CloseFunction(size_t index, bool final)
	{
		if (m_Functions.empty() || index >= m_Functions.size())
			return;

		if (final)
		{
			m_Functions.erase(m_Functions.end() - 1);
			return;
		}

		auto& function = GetCurrentFunction();

		for (size_t i = function.Scopes.size(); i > 0; i--)
		{
			CloseScope(i - 1, final);
		}
	}

	size_t CodeGenerator::OpenScope()
	{
		auto& currentFunction = GetCurrentFunction();

		Scope scope{};
		if (!currentFunction.Scopes.empty())
		{
			const auto& lastScope = GetCurrentScope();
			scope.StackLocation = lastScope.StackLocation + lastScope.ScopeSize;
		}
		if (m_IfBegined)
		{
			scope.ScopeSize++;
			m_IfBegined = false;
		}
		currentFunction.Scopes.emplace_back(std::move(scope));

		m_Code << "; start of scope no. " << currentFunction.Scopes.size() - 1 << "\n";

		return currentFunction.Scopes.size() - 1;
	}

	void CodeGenerator::CloseScope(size_t index, bool final)
	{
		if (m_Functions.empty())
			return;

		auto& currentFunction = GetCurrentFunction();

		if (final)
		{
			currentFunction.Scopes.erase(currentFunction.Scopes.end() - 1);
			return;
		}

		if (currentFunction.Scopes.empty() || index >= currentFunction.Scopes.size())
			return;

		auto& scope = GetCurrentScope();

		m_Code << "\tsub rsp, " << scope.ScopeSize * 8 << "\n";
		m_Code << "; end of scope no. " << currentFunction.Scopes.size() - 1 << "\n";
	}

	void CodeGenerator::PushVariable(const std::string& name)
	{
		auto& scope = GetCurrentScope();
		scope.Variables.emplace_back(Variable{ name, scope.StackLocation });
	}

	std::optional<CodeGenerator::Variable> CodeGenerator::FindVariable(const std::string& name) const
	{
		if (m_Functions.empty())
			return std::nullopt;

		const auto& currentFunction = GetCurrentFunction();

		for (auto it = currentFunction.Scopes.rbegin(); it != currentFunction.Scopes.rend(); it++)
		{
			const auto& scope = *it;

			auto scopeIt = std::find_if(scope.Variables.begin(), scope.Variables.end(), [&name](const Variable& variable)
			{
				return variable.Name == name;
			});

			if (scopeIt != scope.Variables.end())
				return *scopeIt;
		}

		return std::nullopt;
	}

	size_t CodeGenerator::GetStackSize() const
	{
		size_t size = 0;
		for (const auto& function : m_Functions)
			for (const auto& scope : function.Scopes)
				size += scope.ScopeSize;
		return size;
	}

#endif

	void CodeGenerator::StartFunction(const std::string& name)
	{
		if (m_CurrentFunction.has_value())
			throw "You must close the previous function before opening a new one!";

		m_CurrentFunction = Function{};
		m_CurrentFunction->Name = name;
	}

	void CodeGenerator::EndFunction(bool final)
	{
		if (!m_CurrentFunction.has_value())
			return;

		size_t stackSize = 0;
		for (const auto& scope:m_CurrentFunction->Scopes)
			stackSize += scope.ScopeSize;
		Pop(stackSize, final);

		if (final)
		{
			m_CurrentFunction = std::nullopt;
			m_ScopeIndex = m_LabelIndex = 0;
		}
	}

	size_t CodeGenerator::PushScope()
	{
		if (!m_CurrentFunction.has_value())
			throw "You must be in a function!";

		auto currentScope = GetCurrentScope();

		Scope scope{};
		if (currentScope.has_value())
		{
			scope.StackLocation = currentScope.value()->StackLocation + currentScope.value()->ScopeSize;
		}

		m_Code << "; start of scope no. " << m_ScopeIndex << "\n";
		m_CurrentFunction->Scopes.emplace_back(scope);

		return m_ScopeIndex++;
	}

	void CodeGenerator::PopScope(size_t index)
	{
		if (!m_CurrentFunction.has_value() || index >= m_CurrentFunction->Scopes.size())
			return;

		const auto& scope = m_CurrentFunction->Scopes.at(index);
		Pop(scope.ScopeSize);
		m_Code << "; end of scope no. " << index << "\n";

		m_CurrentFunction->Scopes.pop_back();
	}

	void CodeGenerator::DeclareVariable(const std::string& name)
	{
		auto opt = GetCurrentScope();
		if (!opt.has_value())
			throw "Not in a scope!";

		auto scope = opt.value();

		Variable variable{};
		variable.Name = name;
		variable.StackLocation = scope->StackLocation + scope->ScopeSize;
		scope->Variables.emplace_back(variable);
	}

	std::optional<CodeGenerator::Variable> CodeGenerator::FindVariable(const std::string& name) const
	{
		if (!m_CurrentFunction.has_value())
			return std::nullopt;

		for (auto it = m_CurrentFunction->Scopes.rbegin(); it != m_CurrentFunction->Scopes.rend(); it++)
		{
			auto variable = std::find_if(it->Variables.begin(), it->Variables.end(), [&name](const Variable& variable)
			{
				return variable.Name == name;
			});
			if (variable != it->Variables.end())
				return *variable;
		}

		return std::nullopt;
	}

	void CodeGenerator::Push(const std::string_view reg)
	{
		auto scope = GetCurrentScope();
		if (!scope.has_value())
			return;

		m_Code << "\tpush " << reg.data() << "\n";
		scope.value()->ScopeSize++;
	}

	void CodeGenerator::Push(size_t count)
	{
		auto scope = GetCurrentScope();
		if (!scope.has_value())
			return;

		m_Code << "\tsub rsp, " << count * 8 << "\n";
		scope.value()->ScopeSize += count;
	}

	void CodeGenerator::Pop(const std::string_view reg)
	{
		auto scope = GetCurrentScope();
		if (!scope.has_value())
			return;

		m_Code << "\tpop " << reg.data() << "\n";
		scope.value()->ScopeSize--;
	}

	void CodeGenerator::Pop(size_t count, bool final)
	{
		if (!m_CurrentFunction.has_value())
			return;

		m_Code << "\tadd rsp, " << count * 8 << "\n";

		if (!final)
			return;

		for (auto it = m_CurrentFunction->Scopes.rbegin(); it != m_CurrentFunction->Scopes.rend(); it++)
		{
			size_t maxCount = std::min(count, it->ScopeSize);
			it->ScopeSize -= maxCount;
			count -= maxCount;

			if (count == 0)
				return;
		}
	}

	std::optional<CodeGenerator::Scope*> CodeGenerator::GetCurrentScope()
	{
		if (!m_CurrentFunction.has_value() || m_CurrentFunction->Scopes.empty())
			return std::nullopt;

		return &m_CurrentFunction->Scopes.back();
	}

	size_t CodeGenerator::GetStackSize() const
	{
		if (!m_CurrentFunction.has_value())
			return 0;

		size_t size = 0;
		for (const auto& scope : m_CurrentFunction->Scopes)
			size += scope.ScopeSize;
		return size;
	}

}
