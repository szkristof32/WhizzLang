#include "pch.h"
#include "CodeGenerator.h"

namespace WhizzLang {

	std::optional<CodeGenerator::Variable> CodeGenerator::FindVariable(const std::string& name) const
	{
		auto it = std::find_if(m_Variables.begin(), m_Variables.end(), [&name](const Variable& variable)
		{
			return variable.Name == name;
		});
	
		if (it == m_Variables.end())
			return std::nullopt;

		return *it;
	}

	void CodeGenerator::CleanUp()
	{
		for (auto& var : m_Variables)
		{
			Pop("r9");
		}
		m_Variables.clear();
	}

}
