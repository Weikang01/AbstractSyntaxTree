#include "ASTNode.h"

namespace AST
{
	bool ASTNode::operator==(const ASTNode& other) const
	{
		if (mType != other.mType)
			return false;

		switch (mType)
		{
		case NodeType::Rational:
			return *dynamic_cast<const RationalNode*>(this) == *dynamic_cast<const RationalNode*>(&other);
		case NodeType::Irrational:
			return *dynamic_cast<const IrrationalNode*>(this) == *dynamic_cast<const IrrationalNode*>(&other);
		case NodeType::Variable:
			return *dynamic_cast<const VariableNode*>(this) == *dynamic_cast<const VariableNode*>(&other);
		case NodeType::Operator:
			return *dynamic_cast<const OperatorNode*>(this) == *dynamic_cast<const OperatorNode*>(&other);
		case NodeType::Parenthesis:
			return *dynamic_cast<const ParenthesisNode*>(this) == *dynamic_cast<const ParenthesisNode*>(&other);
		default:
			return false;
		}
	}

	bool RationalNode::operator==(const ASTNode& other) const
	{
		if (const auto* rhs = dynamic_cast<const RationalNode*>(&other))
			return mValue == rhs->mValue;
		return false;
	}

	bool AST::IrrationalNode::operator==(const ASTNode& other) const
	{
		if (const auto* rhs = dynamic_cast<const IrrationalNode*>(&other))
			return *mIrrational == *rhs->mIrrational;
		return false;
	}

	bool AST::VariableNode::operator==(const ASTNode& other) const
	{
		if (const auto* rhs = dynamic_cast<const VariableNode*>(&other))
			return *mVariable == *rhs->mVariable;
		return false;
	}

	bool AST::OperatorNode::operator==(const ASTNode& other) const
	{
		if (const auto* rhs = dynamic_cast<const OperatorNode*>(&other))
		{
			if (*mOperator != *rhs->mOperator || mOperands.size() != rhs->mOperands.size())
				return false;

			for (size_t i = 0; i < mOperands.size(); ++i)
			{
				if (*mOperands[i] != *rhs->mOperands[i])
					return false;
			}
			return true;
		}
		return false;
	}

	OperatorNode::~OperatorNode()
	{
		for (auto& operand : mOperands)
		{
			delete operand;
		}
	}

	bool AST::ParenthesisNode::operator==(const ASTNode& other) const
	{
		if (const auto* rhs = dynamic_cast<const ParenthesisNode*>(&other))
			return mParenthesis == rhs->mParenthesis;
		return false;
	}
}