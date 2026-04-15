#pragma once

#include "WhizzLang/Core/Tokeniser/Tokens.h"

namespace WhizzLang {

	class Node
	{
	public:
		virtual ~Node()
		{
			for (const auto& child : m_Children) delete child;
		}

		virtual std::string GenerateCode() const = 0;

		void PushChild(Node* child) { m_Children.emplace_back(child); child->m_Parent = this; }
		std::span<Node*> GetChildren() const { return { m_Children.data(), m_Children.size() }; }
	protected:
		Node* m_Parent = nullptr;
		std::vector<Node*> m_Children;
	};

	class NodeProgram : public Node
	{
	public:
		virtual std::string GenerateCode() const override;
	};

	class NodeFunction : public Node
	{
	public:
		NodeFunction(const Token& identifier, const Token& returnType) : m_Identifier(identifier), m_ReturnType(returnType) {}

		virtual std::string GenerateCode() const override;

		const Token& GetIdentifier() const { return m_Identifier; }
		const Token& GetReturnType() const { return m_ReturnType; }
	private:
		Token m_Identifier;
		Token m_ReturnType;
	};

	class NodeStatement : public Node
	{
	};

	class NodeExpression : public Node
	{
	public:
		NodeExpression(const Token& integer) : m_IntegerLiteral(integer) {}

		virtual std::string GenerateCode() const override;

		const Token& GetIntegerLiteral() const { return m_IntegerLiteral; }
	private:
		const Token& m_IntegerLiteral;
	};

	class NodeReturn : public NodeStatement
	{
	public:
		virtual std::string GenerateCode() const override;
	};

}
