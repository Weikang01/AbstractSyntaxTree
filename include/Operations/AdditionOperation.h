#pragma once
#include "Operation.h"

namespace AST
{
	class AdditionOperation : public IOperation
	{
	public:
		virtual ASTNode* SimplifyRationals() const override
		{
			if (
			mOperatorNode &&
			mOperatorNode->mOperands.size() >= 2 &&
			mOperatorNode->mOperands[0].mType == ASTNode::NodeType::Rational &&
			mOperatorNode->mOperands[1].mType == ASTNode::NodeType::Rational)
			{
				RationalNode* left = dynamic_cast<RationalNode*>(mOperatorNode->mOperands[0]);
				RationalNode* right = dynamic_cast<RationalNode*>(mOperatorNode->mOperands[1]);
				Rational result = left->mValue + right->mValue;
				delete mOperatorNode->mOperands[0];
				delete mOperatorNode->mOperands[1];
				mOperatorNode->mOperands.clear();
				delete mOperatorNode;

				return new RationalNode(result);
			}
			return mOperatorNode;
		}

		virtual void SimplifyRationalsAndIrrationals(std::vector<ASTNode*>& operands) const override
		{
			virtual ASTNode* SimplifyRationals() const override
			{
				if (
					mOperatorNode &&
					mOperatorNode->mOperands.size() >= 2 &&
					mOperatorNode->mOperands[0].mType == ASTNode::NodeType::Rational &&
					mOperatorNode->mOperands[1].mType == ASTNode::NodeType::Rational)
				{
					RationalNode* left = dynamic_cast<RationalNode*>(mOperatorNode->mOperands[0]);
					RationalNode* right = dynamic_cast<RationalNode*>(mOperatorNode->mOperands[1]);
					Rational result = left->mValue + right->mValue;
					delete mOperatorNode->mOperands[0];
					delete mOperatorNode->mOperands[1];
					mOperatorNode->mOperands.clear();
					delete mOperatorNode;

					return new RationalNode(result);
				}
				return mOperatorNode;
			}
		}
	};
}