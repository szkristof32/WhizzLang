#pragma once

namespace WhizzLang {

	class CodeGenerator
	{
	public:
		struct Variable
		{
			std::string Name;
			size_t StackLocation = 0;
		};
		struct Scope
		{
			size_t StackLocation = 0;
			size_t ScopeSize = 0;
			std::vector<Variable> Variables;
		};
	public:
		void OpenScope();
		void CloseScope();

		void PushVariable(const std::string& name);
		std::optional<Variable> FindVariable(const std::string& name) const;

		void Push(const std::string& reg)
		{
			m_Code << "\tpush " << reg << "\n";
			GetCurrentScope().ScopeSize++;
		}
		void Pop(const std::string& reg)
		{
			m_Code << "\tpop " << reg << "\n";
			GetCurrentScope().ScopeSize--;
		}

		Scope& GetCurrentScope() { return m_Scopes.at(m_Scopes.size() - 1); }
		const Scope& GetCurrentScope() const { return m_Scopes.at(m_Scopes.size() - 1); }
		size_t GetStackSize() const { return GetCurrentScope().ScopeSize; }
		const std::stringstream& GetCode() const { return m_Code; }

		template<typename _Ty>
		CodeGenerator& operator<<(const _Ty& rhs)
		{
			m_Code << rhs;
			return *this;
		}
	private:
		std::stringstream m_Code;
		std::vector<Scope> m_Scopes;
	};

}
