#pragma once

namespace WhizzLang {

	class CodeGenerator
	{
	public:
		struct Variable
		{
			std::string Name;
			size_t StackLocation;
		};
		struct Scope
		{
			size_t StackLocation;
			std::vector<Variable> Variables;
		};
		struct Function
		{
			std::string Name;
			size_t StackLocation;
			std::vector<Scope> Scopes;
		};
	public:
		void StartFunction(const std::string& name);
		void EndFunction();

		size_t PushScope();
		void PopScope(size_t index);

		std::optional<Scope> GetCurrentScope();

		template<typename _Ty>
		CodeGenerator& operator<<(const _Ty& rhs)
		{
			m_Code << rhs;
			return *this;
		}
	private:
		std::stringstream m_Code;
		std::optional<Function> m_CurrentFunction = std::nullopt;

		size_t m_ScopeIndex = 0;
		size_t m_LabelIndex = 0;
	};

}
