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

		virtual ASTNode* Clone() const = 0;
	};

	struct RationalNode : public ASTNode
	{
		Rational mValue;

		RationalNode(const Rational& value)
			: mValue(value), ASTNode(NodeType::Rational)
		{
		}

		virtual ASTNode* Clone() const override
		{
			return new RationalNode(mValue);
		}

	};

	struct IrrationalNode : public ASTNode
	{
		const Irrational* mIrrational;
		IrrationalNode(const Symbol* symbol)
			: mIrrational(dynamic_cast<const Irrational*>(symbol)), ASTNode(NodeType::Irrational)
		{
		}

		virtual ASTNode* Clone() const override
		{
			return new IrrationalNode(mIrrational);
		}
	};

	struct VariableNode : public ASTNode
	{
		const Symbol* mVariable;
		VariableNode(const Symbol* symbol)
			: mVariable(symbol), ASTNode(NodeType::Variable)
		{
		}

		virtual ASTNode* Clone() const override
		{
			return new VariableNode(mVariable);
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

		~OperatorNode();

		virtual ASTNode* Clone() const override
		{
			OperatorNode* node = new OperatorNode(mOperator);
			for (auto& operand : mOperands)
			{
				node->mOperands.push_back(operand->Clone());
			}
			return node;
		}
	};

	struct ParenthesisNode : public ASTNode
	{
		const Parenthesis* mParenthesis;
		ParenthesisNode(const Symbol* symbol)
			: mParenthesis(dynamic_cast<const Parenthesis*>(symbol)), ASTNode(NodeType::Parenthesis)
		{
		}

		virtual ASTNode* Clone() const override
		{
			return new ParenthesisNode(mParenthesis);
		}
	};
}