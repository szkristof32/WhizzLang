#include "pch.h"
#include "CodeGenerator.h"

namespace WhizzLang {

	void CodeGenerator::OpenScope()
	{
		Scope scope{};
		if (!m_Scopes.empty())
		{
			const auto& lastScope = GetCurrentScope();
			scope.StackLocation = lastScope.StackLocation + lastScope.ScopeSize;
		}
		m_Scopes.emplace_back(std::move(scope));
	}

	void CodeGenerator::CloseScope()
	{
		if (m_Scopes.empty())
			return;

		auto& scope = GetCurrentScope();

		for (auto& var : scope.Variables)
		{
			Pop("r9");
		}

		m_Scopes.erase(m_Scopes.end() - 1);
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

}
