#include "ASTSimplifierSimplifyRules.h"
#include "ASTNode.h"

namespace AST
{
	struct GeneralSimplify : public ISimplifyRule
	{
		GeneralSimplify() : ISimplifyRule(0) {}

		virtual std::unique_ptr<SimplifyCheckResult> Check(const std::vector<ASTNode*> operands) const override
		{
			if (operands.size() != 1)
			{
				return std::make_unique<SimplifyCheckResult>(false);
			}

			return std::make_unique<SimplifyCheckResult>(true);

		};

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands, const std::unique_ptr<SimplifyCheckResult>& checkResult) const override
		{
			return { true, operands[0]->Clone() };
		}
	};

	UnaryPlusSimplifyRule::UnaryPlusSimplifyRule(ASTSimplifier* simplifier)
		:OperationSimplifyRule(simplifier)
	{
		mRules.AddRule(new GeneralSimplify());
	}
} // namespace AST