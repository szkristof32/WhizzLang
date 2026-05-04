#pragma once

#include "WhizzLang/Core/Tokeniser/Tokens.h"

#include "WhizzLang/Core/CodeGeneration/CodeGenerator.h"

namespace WhizzLang {

	class Node
	{
	public:
		Node(const std::filesystem::path& filename, size_t line, size_t column) : m_Filename(filename), m_Line(line), m_Column(column) {}
		virtual ~Node()
		{
			for (const auto& child : m_Children) delete child;
		}

		virtual void GenerateCode(CodeGenerator& generator) const = 0;

		void PushChild(Node* child) { m_Children.emplace_back(child); child->m_Parent = this; }
		std::span<Node*> GetChildren() const { return { m_Children.data(), m_Children.size() }; }
	protected:
		Node* m_Parent = nullptr;
		std::vector<Node*> m_Children;

		std::filesystem::path m_Filename;
		size_t m_Line = 1;
		size_t m_Column = 1;
	};

	class NodeProgram : public Node
	{
	public:
		NodeProgram(const std::filesystem::path& filename, size_t line, size_t column) : Node(filename, line, column) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeFunction : public Node
	{
	public:
		NodeFunction(const Token& identifier, const Token& returnType, const std::filesystem::path& filename, size_t line, size_t column) 
			: Node(filename, line, column), m_Identifier(identifier), m_ReturnType(returnType) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const Token& GetIdentifier() const { return m_Identifier; }
		const Token& GetReturnType() const { return m_ReturnType; }
	private:
		Token m_Identifier;
		Token m_ReturnType;
	};

	class NodeScope : public Node
	{
	public:
		NodeScope(const std::filesystem::path& filename, size_t line, size_t column) : Node(filename, line, column) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeStatement : public Node
	{
	public:
		NodeStatement(const std::filesystem::path& filename, size_t line, size_t column) : Node(filename, line, column) {}
	};

	class NodeReturn : public NodeStatement
	{
	public:
		NodeReturn(const std::filesystem::path& filename, size_t line, size_t column) : NodeStatement(filename, line, column) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeVariable : public NodeStatement
	{
	public:
		NodeVariable(const Token& identifier, const std::filesystem::path& filename, size_t line, size_t column) 
			: NodeStatement(filename, line, column), m_Identifier(identifier) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const Token& GetIdentifier() const { return m_Identifier; }
	private:
		Token m_Identifier;
	};

	class NodeExpression : public Node
	{
	public:
		NodeExpression(const std::filesystem::path& filename, size_t line, size_t column) : Node(filename, line, column) {}
	};

	class NodeTerm : public NodeExpression
	{
	public:
		NodeTerm(const std::filesystem::path& filename, size_t line, size_t column) : NodeExpression(filename, line, column) {}
	};

	class NodeTermIntegerLiteral : public NodeTerm
	{
	public:
		NodeTermIntegerLiteral(const Token& integer, const std::filesystem::path& filename, size_t line, size_t column) 
			: NodeTerm(filename, line, column), m_IntegerLiteral(integer) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const Token& GetIntegerLiteral() const { return m_IntegerLiteral; }
	private:
		const Token& m_IntegerLiteral;
	};

	class NodeTermBracket : public NodeTerm
	{
	public:
		NodeTermBracket(NodeExpression* expression, const std::filesystem::path& filename, size_t line, size_t column)
			: NodeTerm(filename, line, column), m_Expression(expression) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const NodeExpression* GetExpression() const { return m_Expression; }
	private:
		NodeExpression* m_Expression;
	};

	class NodeTermIdentifier : public NodeTerm
	{
	public:
		NodeTermIdentifier(const Token& identifier, const std::filesystem::path& filename, size_t line, size_t column)
			: NodeTerm(filename, line, column), m_Identifier(identifier) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const Token& GetIdentifier() const { return m_Identifier; }
	private:
		const Token& m_Identifier;
	};

	class NodeBinaryExpression : public NodeExpression
	{
	public:
		NodeBinaryExpression(NodeExpression* lhs, NodeExpression* rhs, const std::filesystem::path& filename, size_t line, size_t column)
			: NodeExpression(filename, line, column), m_Lhs(lhs), m_Rhs(rhs) {}

		NodeExpression* GetLhs() const { return m_Lhs; }
		NodeExpression* GetRhs() const { return m_Rhs; }
	protected:
		NodeExpression* m_Lhs;
		NodeExpression* m_Rhs;
	};

	class NodeBinaryExpressionAdd : public NodeBinaryExpression
	{
	public:
		NodeBinaryExpressionAdd(NodeExpression* lhs, NodeExpression* rhs, const std::filesystem::path& filename, size_t line, size_t column)
			: NodeBinaryExpression(lhs, rhs, filename, line, column) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeBinaryExpressionSubtract : public NodeBinaryExpression
	{
	public:
		NodeBinaryExpressionSubtract(NodeExpression* lhs, NodeExpression* rhs, const std::filesystem::path& filename, size_t line, size_t column)
			: NodeBinaryExpression(lhs, rhs, filename, line, column) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeBinaryExpressionMultiply : public NodeBinaryExpression
	{
	public:
		NodeBinaryExpressionMultiply(NodeExpression* lhs, NodeExpression* rhs, const std::filesystem::path& filename, size_t line, size_t column)
			: NodeBinaryExpression(lhs, rhs, filename, line, column) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeBinaryExpressionDivide : public NodeBinaryExpression
	{
	public:
		NodeBinaryExpressionDivide(NodeExpression* lhs, NodeExpression* rhs, const std::filesystem::path& filename, size_t line, size_t column)
			: NodeBinaryExpression(lhs, rhs, filename, line, column) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

}
