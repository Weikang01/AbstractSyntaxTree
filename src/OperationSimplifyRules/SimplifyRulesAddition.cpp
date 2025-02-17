#include "ASTSimplifierSimplifyRules.h"
#include "ASTNode.h"

namespace AST
{
	struct RationalSimplify : public ISimplifyRule
	{
		RationalSimplify() : ISimplifyRule(0) {}

		virtual std::unique_ptr<SimplifyCheckResult> Check(const std::vector<ASTNode*> operands) const
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

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands, const std::unique_ptr<SimplifyCheckResult>& checkResult) const
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

	// left * common + right * common = (left + right) * common
	ASTNode* AdditionSimplifyRule::CombineCommonTerms(const ASTNode* left, const ASTNode* right, const ASTNode* common)
	{
		if (!left || !right || !common)
			return nullptr; // Ensure valid input

		std::vector<ASTNode*> operands = { left->Clone(), right->Clone() };
		SimplifyResult additionSimplifiedResult = mSimplifier->SimplifyOperation(OperationId::Addition, operands);
		ASTNode* additionSimplifiedNode = additionSimplifiedResult.mSuccess ? additionSimplifiedResult.mResult : NewOperatorNode(OperationId::Addition, operands);

		operands = { additionSimplifiedNode, common->Clone() };
		SimplifyResult multiplicationSimplifiedResult = mSimplifier->SimplifyOperation(OperationId::Multiplication, operands);
		ASTNode* multiplicationSimplifiedNode = multiplicationSimplifiedResult.mSuccess ? multiplicationSimplifiedResult.mResult : NewOperatorNode(OperationId::Multiplication, operands);

		return multiplicationSimplifiedNode;
	}

	struct LikeTermSimplify : public ISimplifyRule
	{
		AdditionSimplifyRule* mSimplifyRule = nullptr;

		LikeTermSimplify(AdditionSimplifyRule* simplifyRule) : ISimplifyRule(2), mSimplifyRule(simplifyRule) {}

		struct LikeTermSimplifyCheckResult : public SimplifyCheckResult
		{
			bool mIsOperand0Multiplication = false;
			bool mOperand0MatchedIndex = false;  // false = 0, true = 1
			bool mIsOperand1Multiplication = false;
			bool mOperand1MatchedIndex = false;  // false = 0, true = 1

			LikeTermSimplifyCheckResult() = default;
			LikeTermSimplifyCheckResult(const bool success, const bool isOperand0Multiplication, const bool operand0MatchedIndex,
				const bool isOperand1Multiplication, const bool operand1MatchedIndex)
				: SimplifyCheckResult(success),
				mIsOperand0Multiplication(isOperand0Multiplication), mOperand0MatchedIndex(operand0MatchedIndex),
				mIsOperand1Multiplication(isOperand1Multiplication), mOperand1MatchedIndex(operand1MatchedIndex)
			{
			};
		};

		virtual std::unique_ptr<SimplifyCheckResult> Check(const std::vector<ASTNode*> operands) const override
		{
			if (operands.size() != 2)
			{
				return std::make_unique<LikeTermSimplifyCheckResult>(false, false, false, false, false);
			}

			bool isOperand0Multiplication = false;
			bool operand0MatchedIndex = false;
			bool isOperand1Multiplication = false;
			bool operand1MatchedIndex = false;
			bool success = false;

			if (operands[0]->mType == ASTNode::NodeType::Operator)
			{
				const OperatorNode* operatorNode = dynamic_cast<const OperatorNode*>(operands[0]);
				if (operatorNode->mOperator->mOperationId == OperationId::Multiplication)
				{
					isOperand0Multiplication = true;
					if (operands[1]->mType == ASTNode::NodeType::Operator)
					{
						const OperatorNode* operatorNode2 = dynamic_cast<const OperatorNode*>(operands[1]);
						if (operatorNode2->mOperator->mOperationId == OperationId::Multiplication)
						{
							isOperand1Multiplication = true;

							if (*operatorNode->mOperands[0] == *operatorNode2->mOperands[0])
							{
								operand0MatchedIndex = false;
								operand1MatchedIndex = false;
								success = true;
							}
							else if (*operatorNode->mOperands[0] == *operatorNode2->mOperands[1])
							{
								operand0MatchedIndex = false;
								operand1MatchedIndex = true;
								success = true;
							}
							else if (*operatorNode->mOperands[1] == *operatorNode2->mOperands[0])
							{
								operand0MatchedIndex = true;
								operand1MatchedIndex = false;
								success = true;
							}
							else if (*operatorNode->mOperands[1] == *operatorNode2->mOperands[1])
							{
								operand0MatchedIndex = true;
								operand1MatchedIndex = true;
								success = true;
							}
						}
						else
						{
							if (*operatorNode->mOperands[0] == *operands[1])
							{
								operand0MatchedIndex = false;
								success = true;
							}
							else if (*operatorNode->mOperands[1] == *operands[1])
							{
								operand0MatchedIndex = true;
								success = true;
							}
						}
					}
					else
					{
						if (*operatorNode->mOperands[0] == *operands[1])
						{
							operand0MatchedIndex = false;
							success = true;
						}
						else if (*operatorNode->mOperands[1] == *operands[1])
						{
							operand0MatchedIndex = true;
							success = true;
						}
					}

					return std::make_unique<LikeTermSimplifyCheckResult>(
						success, isOperand0Multiplication, operand0MatchedIndex,
						isOperand1Multiplication, operand1MatchedIndex);
				}
			}

			if (operands[1]->mType == ASTNode::NodeType::Operator)
			{
				const OperatorNode* operatorNode2 = dynamic_cast<const OperatorNode*>(operands[1]);
				if (operatorNode2->mOperator->mOperationId == OperationId::Multiplication)
				{
					isOperand1Multiplication = true;

					if (*operands[0] == *operatorNode2->mOperands[0])
					{
						operand1MatchedIndex = false;
						success = true;
					}
					else if (*operands[0] == *operatorNode2->mOperands[1])
					{
						operand1MatchedIndex = true;
						success = true;
					}
				}
				else
				{
					if (*operands[0] == *operatorNode2->mOperands[0])
					{
						success = true;
					}
				}
			}
			else
			{
				if (*operands[0] == *operands[1])
				{
					success = true;
				}
			}

			return std::make_unique<LikeTermSimplifyCheckResult>(
				success, isOperand0Multiplication, operand0MatchedIndex,
				isOperand1Multiplication, operand1MatchedIndex);
		}

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands, const std::unique_ptr<SimplifyCheckResult>& checkResult) const override
		{
			const LikeTermSimplifyCheckResult* likeTermResult = dynamic_cast<const LikeTermSimplifyCheckResult*>(checkResult.get());
			if (!likeTermResult || !likeTermResult->mSuccess)
			{
				return SimplifyResult(false, nullptr); // If the check failed, return failure
			}

			// Extract the operands
			ASTNode* operand0 = operands[0];
			ASTNode* operand1 = operands[1];

			// Determine the common term and the remaining terms
			ASTNode* commonTerm = nullptr;
			ASTNode* leftTerm = nullptr;
			ASTNode* rightTerm = nullptr;

			if (likeTermResult->mIsOperand0Multiplication && likeTermResult->mIsOperand1Multiplication)
			{
				// Both operands are multiplications
				const OperatorNode* operatorNode0 = dynamic_cast<const OperatorNode*>(operand0);
				const OperatorNode* operatorNode1 = dynamic_cast<const OperatorNode*>(operand1);

				// Determine which operand of the first multiplication matches which operand of the second multiplication
				if (likeTermResult->mOperand0MatchedIndex == likeTermResult->mOperand1MatchedIndex)
				{
					// Both matched operands are at the same index (e.g., left * common and left * common)
					commonTerm = operatorNode0->mOperands[likeTermResult->mOperand0MatchedIndex]->Clone();
					leftTerm = operatorNode0->mOperands[!likeTermResult->mOperand0MatchedIndex]->Clone();
					rightTerm = operatorNode1->mOperands[!likeTermResult->mOperand1MatchedIndex]->Clone();
				}
				else
				{
					// Matched operands are at different indices (e.g., left * common and common * right)
					commonTerm = operatorNode0->mOperands[likeTermResult->mOperand0MatchedIndex]->Clone();
					leftTerm = operatorNode0->mOperands[!likeTermResult->mOperand0MatchedIndex]->Clone();
					rightTerm = operatorNode1->mOperands[!likeTermResult->mOperand1MatchedIndex]->Clone();
				}
			}
			else if (likeTermResult->mIsOperand0Multiplication)
			{
				// Only operand0 is a multiplication
				const OperatorNode* operatorNode0 = dynamic_cast<const OperatorNode*>(operand0);
				commonTerm = operatorNode0->mOperands[likeTermResult->mOperand0MatchedIndex]->Clone();
				leftTerm = operatorNode0->mOperands[!likeTermResult->mOperand0MatchedIndex]->Clone();
				rightTerm = mSimplifyRule->NewRationalNode(1);
			}
			else if (likeTermResult->mIsOperand1Multiplication)
			{
				// Only operand1 is a multiplication
				const OperatorNode* operatorNode1 = dynamic_cast<const OperatorNode*>(operand1);
				commonTerm = operatorNode1->mOperands[likeTermResult->mOperand1MatchedIndex]->Clone();
				leftTerm = mSimplifyRule->NewRationalNode(1);
				rightTerm = operatorNode1->mOperands[!likeTermResult->mOperand1MatchedIndex]->Clone();
			}
			else
			{
				// Neither operand is a multiplication (e.g., x + x)
				commonTerm = operand0->Clone();
				leftTerm = mSimplifyRule->NewRationalNode(1); // 1 * x + 1 * x = (1 + 1) * x
				rightTerm = mSimplifyRule->NewRationalNode(1);
			}

			// Combine the terms using the CombineCommonTerms function

			ASTNode* simplifiedNode = mSimplifyRule->CombineCommonTerms(leftTerm, rightTerm, commonTerm);
			if (!simplifiedNode)
			{
				return SimplifyResult(false, nullptr); // If combining failed, return failure
			}

			return SimplifyResult(true, simplifiedNode);
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
		OppositeSimplify() : ISimplifyRule(1) {}

		virtual std::unique_ptr<SimplifyCheckResult> Check(const std::vector<ASTNode*> operands) const override
		{
			if (operands.size() != 2)
			{
				return std::make_unique<SimplifyCheckResult>(false);
			}

			return std::make_unique<SimplifyCheckResult>(IsUnaryMinus(operands[0]) || IsUnaryMinus(operands[1]));
		}

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands, const std::unique_ptr<SimplifyCheckResult>& checkResult) const override
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

	AdditionSimplifyRule::AdditionSimplifyRule(ASTSimplifier* simplifier)
		:OperationSimplifyRule(simplifier)
	{
		mRules.AddRule(new IdentitySimplify());
		mRules.AddRule(new RationalSimplify());
		mRules.AddRule(new OppositeSimplify());
		mRules.AddRule(new LikeTermSimplify(this));
	}
}