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
	public:
		std::optional<Variable> FindVariable(const std::string& name) const;
		void PushVariable(const std::string& name) { m_Variables.emplace_back(Variable{ name, m_StackSize }); }
		void CleanUp();

		void Push(const std::string& reg)
		{
			m_Code << "\tpush " << reg << "\n";
			m_StackSize++;
		}
		void Pop(const std::string& reg)
		{
			m_Code << "\tpop " << reg << "\n";
			m_StackSize--;
		}

		size_t GetStackSize() const { return m_StackSize; }
		const std::stringstream& GetCode() const { return m_Code; }

		template<typename _Ty>
		CodeGenerator& operator<<(const _Ty& rhs)
		{
			m_Code << rhs;
			return *this;
		}
	private:
		std::stringstream m_Code;
		std::vector<Variable> m_Variables;
		size_t m_StackSize = 0;
	};

}
