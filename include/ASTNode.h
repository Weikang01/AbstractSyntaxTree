#pragma once
#include <vector>

#include "Rational.h"
#include "Symbol.h"
#include "Operator.h"
#include "Irrational.h"
#include "Parenthesis.h"

namespace AST
{
	struct ASTNode
	{
	public:
		enum class NodeType : uint8_t
		{
			Unknown,
			Rational,
			Irrational,
			Operator,
			Variable,
			Parenthesis,
		};

		NodeType mType = NodeType::Unknown;

		ASTNode(const NodeType type) : mType(type) {}

		virtual ~ASTNode() = default;
	};

	struct RationalNode : public ASTNode
	{
		Rational mValue;

		RationalNode(const Rational& value)
			: mValue(value), ASTNode(NodeType::Rational) {
		}
	};

	struct IrrationalNode : public ASTNode
	{
		const Irrational* mIrrational;
		IrrationalNode(const Symbol* symbol)
			: mIrrational(dynamic_cast<const Irrational*>(symbol)), ASTNode(NodeType::Irrational)
		{
		}
	};

	struct VariableNode : public ASTNode
	{
		const Symbol* mVariable;
		VariableNode(const Symbol* symbol)
			: mVariable(symbol), ASTNode(NodeType::Variable)
		{
		}
	};

	struct OperatorNode : public ASTNode
	{
		// support for unary, binary, function singular, function dual, function multiple
		const Operator* mOperator;
		std::vector<ASTNode*> mOperands;
		OperatorNode(const Symbol* symbol)
			: mOperator(dynamic_cast<const Operator*>(symbol)), ASTNode(NodeType::Operator)
		{
		}
	};

	struct ParenthesisNode : public ASTNode
	{
		const Parenthesis* mParenthesis;
		ParenthesisNode(const Symbol* symbol)
			: mParenthesis(dynamic_cast<const Parenthesis*>(symbol)), ASTNode(NodeType::Parenthesis)
		{
		}
	};
}