#include "ASTNode.h"

namespace AST
{
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