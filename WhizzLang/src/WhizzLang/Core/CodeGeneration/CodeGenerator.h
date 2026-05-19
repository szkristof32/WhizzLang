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
		struct Function
		{
			size_t ScopeCount;
			std::vector<Scope> Scopes;
		};
	public:
		size_t OpenScope();
		void CloseScope(size_t index);
		void CloseAllScopes();

		void BeginIf() { m_IfBegined = true; }

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
		void Pop()
		{
			m_Code << "\tsub rsp, 4\n";
			GetCurrentScope().ScopeSize--;
		}

		size_t NewLabel() { return m_LabelCount++; }

		Scope& GetCurrentScope() { return m_Scopes.at(m_Scopes.size() - 1); }
		const Scope& GetCurrentScope() const { return m_Scopes.at(m_Scopes.size() - 1); }
		size_t GetCurrentScopeIndex() const { return m_Scopes.size() - 1; }

		size_t GetStackSize() const;
		const std::stringstream& GetCode() const { return m_Code; }

		template<typename _Ty>
		CodeGenerator& operator<<(const _Ty& rhs)
		{
			m_Code << rhs;
			return *this;
		}
	private:
		std::stringstream m_Code;
		std::vector<Function> m_Scopes;
		size_t m_LabelCount = 1;
		bool m_IfBegined = false;
	};

}
