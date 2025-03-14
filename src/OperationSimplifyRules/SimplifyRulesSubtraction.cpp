#include "ASTSimplifierSimplifyRules.h"
#include "ASTNode.h"

namespace AST
{
	struct RationalSimplify : public ISimplifyRule
	{
		RationalSimplify() : ISimplifyRule(0) {}

		virtual std::unique_ptr<SimplifyCheckResult> Check(const std::vector<ASTNode*> operands) const override
		{
			for (auto& operand : operands)
			{
				if (operand->mType != ASTNode::NodeType::Rational)
				{
					return std::make_unique<SimplifyCheckResult>(false);
				}
			}
			return std::make_unique<SimplifyCheckResult>(true);
		}

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands, const std::unique_ptr<SimplifyCheckResult>& checkResult) const override
		{
			Rational result = 0;
			for (auto& operand : operands)
			{
				RationalNode* rationalNode = dynamic_cast<RationalNode*>(operand);
				result -= rationalNode->mValue;
			}
			return { true, new RationalNode(result) };
		}
	};

	struct IdentitySimplify : public ISimplifyRule
	{
		IdentitySimplify() : ISimplifyRule(0) {}

		virtual std::unique_ptr<SimplifyCheckResult> Check(const std::vector<ASTNode*> operands) const override
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

			return std::make_unique<SimplifyCheckResult>(nonZeroOperands == 1);

		};

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands, const std::unique_ptr<SimplifyCheckResult>& checkResult) const override
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

	SubtractionSimplifyRule::SubtractionSimplifyRule(ASTSimplifier* simplifier)
		:OperationSimplifyRule(simplifier)
	{
		mRules.AddRule(new IdentitySimplify());
		mRules.AddRule(new RationalSimplify());
	}
}