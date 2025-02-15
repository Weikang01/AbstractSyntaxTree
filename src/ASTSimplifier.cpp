#include "ASTSimplifier.h"
#include "ASTSimplifierSimplifyRules.h"
#include "ASTNode.h"

namespace AST
{
#pragma region Commented
	//struct Addition : public IOperationSimplifyRule
	//{
	//public:
	//	SimplifyResult Simplify(const ASTNode* node) const override
	//	{
	//		if (node->mType != ASTNode::NodeType::Operator)
	//		{
	//			return { false, nullptr };
	//		}

	//		const OperatorNode* opNode = dynamic_cast<const OperatorNode*>(node);

	//		if (opNode->mOperands.size() == 0)
	//		{
	//			return { true, new RationalNode(0) };
	//		}
	//	}

	//	struct RationalSimplify : public ISimplifyRule
	//	{
	//		SimplifyResult Simplify(const ASTNode* node) const override
	//		{
	//			
	//			const OperatorNode* opNode = dynamic_cast<const OperatorNode*>(node);

	//			Rational result = 0;
	//			for (auto& operand : opNode->mOperands)
	//			{
	//				if (operand->mType != ASTNode::NodeType::Rational)
	//				{
	//					return { false, nullptr };
	//				}
	//				RationalNode* rationalNode = dynamic_cast<RationalNode*>(operand);
	//				result += rationalNode->mValue;
	//			}
	//			return { true, new RationalNode(result) };
	//		};
	//	}

	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		Rational result = operands[0];
	//		for (size_t i = 1; i < operands.size(); ++i)
	//		{
	//			result += operands[i];
	//		}
	//		return { true, new RationalNode(result)};
	//	}
	//};

	//struct UnaryPlus : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		return { true, operands[0] };
	//	}
	//};

	//struct Subtraction : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		Rational result = operands[0];
	//		for (size_t i = 1; i < operands.size(); ++i)
	//		{
	//			result -= operands[i];
	//		}
	//		return { true, result };
	//	}
	//};

	//struct UnaryMinus : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		return { true, -operands[0] };
	//	}
	//};

	//struct Multiplication : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		Rational result = operands[0];
	//		for (size_t i = 1; i < operands.size(); ++i)
	//		{
	//			result *= operands[i];
	//		}
	//		return { true, result };
	//	}
	//};

	//struct Division : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		Rational result = operands[0];
	//		for (size_t i = 1; i < operands.size(); ++i)
	//		{
	//			result /= operands[i];
	//		}
	//		return { true, result };
	//	}
	//};

	//struct Factorial : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands[0].GetDenominator() != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		Rational result = 1;
	//		for (Rational i = 2; i <= operands[0]; ++i)
	//		{
	//			result *= i;
	//		}
	//		return { true, result };
	//	}
	//};

	//struct Exponentiation : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 2 || !operands[0].CanYieldRationalPowerResult(operands[1]))
	//		{
	//			return { false, Rational() };
	//		}
	//		return { true, operands[0].Pow(operands[1]) };
	//	}
	//};

	//struct Root : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 2)
	//		{
	//			return { false, Rational() };
	//		}

	//		Rational invertedExponent = operands[1].Inverse();
	//		if (!operands[0].CanYieldRationalPowerResult(invertedExponent))
	//		{
	//			return { false, Rational() };
	//		}

	//		return { true, operands[0].Pow(invertedExponent) };
	//	}
	//};

	//struct Sine : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 1)
	//		{
	//			return { false, Rational() };
	//		}

	//		if (operands[0] != 0)
	//		{
	//			return { false, Rational() };
	//		}

	//		return { true, Rational(0) };
	//	}
	//};

	//struct Cosine : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		if (operands[0] != 0)
	//		{
	//			return { false, Rational() };
	//		}
	//		return { true, Rational(1) };
	//	}
	//};

	//struct Tangent : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		if (operands[0] != 0)
	//		{
	//			return { false, Rational() };
	//		}
	//		return { true, Rational(0) };
	//	}
	//};

	//struct ArcSine : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		if (operands[0] != 0)
	//		{
	//			return { false, Rational() };
	//		}
	//		return { true, Rational(0) };
	//	}
	//};

	//struct ArcCosine : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		if (operands[0] != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		return { true, Rational(0) };
	//	}
	//};

	//struct ArcTangent : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		if (operands[0] != 0)
	//		{
	//			return { false, Rational() };
	//		}
	//		return { true, Rational(0) };
	//	}
	//};

	//struct HyperbolicSine : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		if (operands[0] != 0)
	//		{
	//			return { false, Rational() };
	//		}
	//		return { true, Rational(0) };
	//	}
	//};

	//struct HyperbolicCosine : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		if (operands[0] != 0)
	//		{
	//			return { false, Rational() };
	//		}
	//		return { true, Rational(1) };
	//	}
	//};

	//struct HyperbolicTangent : public IOperationSimplifyRule
	//{
	//	SimplifyResult RationalSimplify(const std::vector<Rational>& operands) const override
	//	{
	//		if (operands.size() != 1)
	//		{
	//			return { false, Rational() };
	//		}
	//		if (operands[0] != 0)
	//		{
	//			return { false, Rational() };
	//		}
	//		return { true, Rational(0) };
	//	}
	//};
#pragma endregion

	void OrderedSimplifyRuleList::AddRule(ISimplifyRule* rule)
	{
		auto it = std::lower_bound(mRules.begin(), mRules.end(), rule,
			[](const ISimplifyRule* a, const ISimplifyRule* b) {
				return a->mPriority < b->mPriority; // Lowest priority first
			});
		mRules.insert(it, rule);
	}

	void ASTSimplifier::BindSimplifyRule(const OperationId operation, const OperationSimplifyRule* rule)
	{
		mSimplifyRules[operation] = rule;
	}

	ASTNode* ASTSimplifier::Simplify(ASTNode* node) const
	{
		if (node->mType == ASTNode::NodeType::Operator)
		{
			OperatorNode* opNode = dynamic_cast<OperatorNode*>(node);
			std::vector<ASTNode*> newOperands;
			for (ASTNode* operand : opNode->mOperands)
			{
				newOperands.push_back(Simplify(operand));
			}
			auto ruleIt = mSimplifyRules.find(opNode->mOperator->mOperationId);
			if (ruleIt != mSimplifyRules.end())
			{
				SimplifyResult result = ruleIt->second->Simplify(newOperands);
				if (result.mSuccess)
				{
					return result.mResult;
				}
				else
				{
					return opNode->CloneWithNewOperands(newOperands);
				}
			}
			else
			{
				return opNode->CloneWithNewOperands(newOperands);
			}
		}
		else
		{
			return node->Clone();
		}

		return nullptr; // temp
	}

	const ASTSimplifier& ASTSimplifier::GetDefault()
	{
		static ASTSimplifier instance;

		instance.BindSimplifyRule(OperationId::Addition, new AdditionSimplifyRule());

		return instance;
	}

	SimplifyResult OperationSimplifyRule::Simplify(const std::vector<ASTNode*> operands) const
	{
		for (auto rule : mRules)
		{
			if (rule->Check(operands))
			{
				return rule->Simplify(operands);
			}
		}
		return { false, nullptr };
	}
}
