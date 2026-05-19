#include "pch.h"
#include "CodeGenerator.h"

namespace WhizzLang {

	size_t CodeGenerator::OpenScope()
	{
		Scope scope{};
		if (!m_Scopes.empty())
		{
			const auto& lastScope = GetCurrentScope();
			scope.StackLocation = lastScope.StackLocation + lastScope.ScopeSize;
		}
		if (m_IfBegined)
		{
			scope.ScopeSize++;
			m_IfBegined = false;
		}
		m_Scopes.emplace_back(std::move(scope));

		m_Code << "; start of scope no. " << m_Scopes.size() - 1 << "\n";

		return m_Scopes.size() - 1;
	}

	void CodeGenerator::CloseScope(size_t index)
	{
		if (m_Scopes.empty() || index >= m_Scopes.size())
			return;

		auto& scope = GetCurrentScope();

		m_Code << "\tsub rsp, " << scope.ScopeSize * 4 << "\n";
		m_Code << "; end of scope no. " << m_Scopes.size() - 1 << "\n";

		m_Scopes.erase(m_Scopes.end() - 1);
	}

	void CodeGenerator::CloseAllScopes()
	{
		for (size_t i = m_Scopes.size(); i > 0; i--)
		{
			CloseScope(i - 1);
		}
	}

	void CodeGenerator::PushVariable(const std::string& name)
	{
		auto& scope = GetCurrentScope();
		scope.Variables.emplace_back(Variable{ name, scope.StackLocation });
	}

	std::optional<CodeGenerator::Variable> CodeGenerator::FindVariable(const std::string& name) const
	{
		for (auto it = m_Scopes.rbegin(); it != m_Scopes.rend(); it++)
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
		for (const auto& scope : m_Scopes)
			size += scope.ScopeSize;
		return size;
	}

}
