#pragma once

#include "WhizzLang/Core/Tokeniser/Tokens.h"

#include "WhizzLang/Core/CodeGeneration/CodeGenerator.h"

namespace WhizzLang {

	class Node
	{
	public:
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
	};

	class NodeProgram : public Node
	{
	public:
		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeFunction : public Node
	{
	public:
		NodeFunction(const Token& identifier, const Token& returnType) : m_Identifier(identifier), m_ReturnType(returnType) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const Token& GetIdentifier() const { return m_Identifier; }
		const Token& GetReturnType() const { return m_ReturnType; }
	private:
		Token m_Identifier;
		Token m_ReturnType;
	};

	class NodeStatement : public Node
	{
	};

	class NodeReturn : public NodeStatement
	{
	public:
		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeVariable : public NodeStatement
	{
	public:
		NodeVariable(const Token& identifier) : m_Identifier(identifier) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const Token& GetIdentifier() const { return m_Identifier; }
	private:
		Token m_Identifier;
	};

	class NodeExpression : public Node
	{
	};

	class NodeTerm : public NodeExpression
	{
	};

	class NodeTermIntegerLiteral : public NodeTerm
	{
	public:
		NodeTermIntegerLiteral(const Token& integer) : m_IntegerLiteral(integer) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const Token& GetIntegerLiteral() const { return m_IntegerLiteral; }
	private:
		const Token& m_IntegerLiteral;
	};

	class NodeTermBracket : public NodeTerm
	{
	public:
		NodeTermBracket(NodeExpression* expression) : m_Expression(expression) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const NodeExpression* GetExpression() const { return m_Expression; }
	private:
		NodeExpression* m_Expression;
	};

	class NodeTermIdentifier : public NodeTerm
	{
	public:
		NodeTermIdentifier(const Token& identifier) : m_Identifier(identifier) {}

		virtual void GenerateCode(CodeGenerator& generator) const override;

		const Token& GetIdentifier() const { return m_Identifier; }
	private:
		const Token& m_Identifier;
	};

	class NodeBinaryExpression : public NodeExpression
	{
	public:
		NodeBinaryExpression(NodeExpression* lhs, NodeExpression* rhs)
			: m_Lhs(lhs), m_Rhs(rhs)
		{
		}

		NodeExpression* GetLhs() const { return m_Lhs; }
		NodeExpression* GetRhs() const { return m_Rhs; }
	protected:
		NodeExpression* m_Lhs;
		NodeExpression* m_Rhs;
	};

	class NodeBinaryExpressionAdd : public NodeBinaryExpression
	{
	public:
		NodeBinaryExpressionAdd(NodeExpression* lhs, NodeExpression* rhs)
			: NodeBinaryExpression(lhs, rhs)
		{
		}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeBinaryExpressionSubtract : public NodeBinaryExpression
	{
	public:
		NodeBinaryExpressionSubtract(NodeExpression* lhs, NodeExpression* rhs)
			: NodeBinaryExpression(lhs, rhs)
		{
		}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeBinaryExpressionMultiply : public NodeBinaryExpression
	{
	public:
		NodeBinaryExpressionMultiply(NodeExpression* lhs, NodeExpression* rhs)
			: NodeBinaryExpression(lhs, rhs)
		{
		}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

	class NodeBinaryExpressionDivide : public NodeBinaryExpression
	{
	public:
		NodeBinaryExpressionDivide(NodeExpression* lhs, NodeExpression* rhs)
			: NodeBinaryExpression(lhs, rhs)
		{
		}

		virtual void GenerateCode(CodeGenerator& generator) const override;
	};

}
