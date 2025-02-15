#include "ASTSimplifierSimplifyRules.h"
#include "ASTNode.h"

namespace AST
{
	struct RationalSimplify : public ISimplifyRule
	{
		RationalSimplify() : ISimplifyRule(0) {}

		virtual bool Check(const std::vector<ASTNode*> operands) const
		{
			for (auto& operand : operands)
			{
				if (operand->mType != ASTNode::NodeType::Rational)
				{
					return false;
				}
			}
			return true;
		}

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands) const
		{
			Rational result = 0;
			for (auto& operand : operands)
			{
				RationalNode* rationalNode = dynamic_cast<RationalNode*>(operand);
				result += rationalNode->mValue;
			}
			return { true, new RationalNode(result) };
		}
	};

	struct IdentitySimplify : public ISimplifyRule
	{
		IdentitySimplify() : ISimplifyRule(0) {}

		virtual bool Check(const std::vector<ASTNode*> operands) const
		{
			int32_t nonZeroOperands = 0;
			for (auto& operand : operands)
			{
				if (operand->mType != ASTNode::NodeType::Rational)
				{
					++nonZeroOperands;
					continue;
				}
				RationalNode* rationalNode = dynamic_cast<RationalNode*>(operand);
				if (rationalNode->mValue != 0)
				{
					++nonZeroOperands;
				}
			}

			return nonZeroOperands == 1;

		};

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands) const
		{
			for (auto& operand : operands)
			{
				if (operand->mType != ASTNode::NodeType::Rational)
				{
					return { true, operand->Clone() };
				}

				RationalNode* rationalNode = dynamic_cast<RationalNode*>(operand);
				if (rationalNode->mValue != 0)
				{
					return { true, operand->Clone() };
				}
			}
			return { true, new RationalNode(0) };
		}
	};

	AdditionSimplifyRule::AdditionSimplifyRule()
	{
		mRules.AddRule(new IdentitySimplify());
		mRules.AddRule(new RationalSimplify());
	}
}