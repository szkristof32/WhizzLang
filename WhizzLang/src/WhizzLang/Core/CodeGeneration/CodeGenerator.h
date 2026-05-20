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
			size_t ScopeSize;
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
		void EndFunction(bool final = true);

		size_t PushScope();
		void PopScope(size_t index);

		void DeclareVariable(const std::string& name, bool growStack = false);
		std::optional<Variable> FindVariable(const std::string& name) const;

		void Push(const std::string_view reg);
		void Push(size_t count = 1);
		void Pop(const std::string_view reg);
		void Pop(size_t count = 1, bool final = true);

		std::optional<Scope*> GetCurrentScope();
		size_t GetStackSize() const;

		size_t NewLabel() { return m_LabelIndex++; }

		void SetLabelContext(size_t label) { m_LabelContext = label; }
		size_t GetLabelContext() const { return m_LabelContext; }

		const std::stringstream& GetCode() const { return m_Code; }

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

		size_t m_LabelContext = 0;
	};

}
