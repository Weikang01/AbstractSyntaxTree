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

	// left * common + right * common = (left + right) * common
	//ASTNode* CombineCommonTerms(const ASTNode* left, const ASTNode* right, const ASTNode* common)
	//{

	//}

	struct SameTermSimplify : public ISimplifyRule
	{
		SameTermSimplify() : ISimplifyRule(1) {}
		virtual bool Check(const std::vector<ASTNode*> operands) const
		{
			if (operands.size() != 2)
			{
				return false;
			}
			return *operands[0] == *operands[1];
		}
		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands) const
		{
			return { true, operands[0]->Clone() };
		}
	};

	struct OppositeSimplify : public ISimplifyRule
	{
	private:
		bool IsUnaryMinus(const ASTNode* node) const
		{
			if (node->mType != ASTNode::NodeType::Operator)
			{
				return false;
			}
			const OperatorNode* operatorNode = dynamic_cast<const OperatorNode*>(node);
			if (operatorNode->mOperator->mOperationId != OperationId::UnaryMinus)
			{
				return false;
			}
			return true;
		}

		const ASTNode* GetUnaryMinusOperand(const ASTNode* node) const
		{
			const OperatorNode* operatorNode = dynamic_cast<const OperatorNode*>(node);
			return operatorNode->mOperands[0];
		}

	public:
		OppositeSimplify() : ISimplifyRule(2) {}

		virtual bool Check(const std::vector<ASTNode*> operands) const
		{
			if (operands.size() != 2)
			{
				return false;
			}

			return IsUnaryMinus(operands[0]) || IsUnaryMinus(operands[1]);
		}

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands) const
		{
			if (IsUnaryMinus(operands[0]))
			{
				if (*GetUnaryMinusOperand(operands[0]) == *operands[1])
				{
					return { true, new RationalNode(0) };
				}
			}
			else
			{
				if (*GetUnaryMinusOperand(operands[1]) == *operands[0])
				{
					return { true, new RationalNode(0) };
				}
			}
			return { false, nullptr };
		}
	};

	AdditionSimplifyRule::AdditionSimplifyRule()
	{
		mRules.AddRule(new IdentitySimplify());
		mRules.AddRule(new RationalSimplify());
		mRules.AddRule(new OppositeSimplify());
	}
}