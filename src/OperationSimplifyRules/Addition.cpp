#include "OperationSimplifyRules/Addition.h"
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

	AdditionSimplifyRule::AdditionSimplifyRule()
	{
		mRules.AddRule(new RationalSimplify());
	}
}