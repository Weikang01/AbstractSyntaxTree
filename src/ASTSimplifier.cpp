#include "ASTSimplifier.h"
#include "ASTSimplifierSimplifyRules.h"
#include "ASTNode.h"

namespace AST
{
	void OrderedSimplifyRuleList::AddRule(ISimplifyRule* rule)
	{
		auto it = std::lower_bound(mRules.begin(), mRules.end(), rule,
			[](const ISimplifyRule* a, const ISimplifyRule* b) {
				// First, compare by priority
				if (a->mPriority != b->mPriority) {
					return a->mPriority < b->mPriority; // Lower priority comes first
				}
				// If priorities are equal, compare by memory address to preserve insertion order
				return a < b;
			});
		mRules.insert(it, rule);
	}

	RationalNode* OperationSimplifyRule::NewRationalNode(const Rational& value) const
	{
		return new RationalNode(value);
	}

	OperatorNode* OperationSimplifyRule::NewOperatorNode(const OperationId operationId, const std::vector<ASTNode*>& operands) const
	{
		return new OperatorNode(mSimplifier->GetSettings().mOperatorRegistry->GetOperator(operationId).get(), operands);
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

	SimplifyResult ASTSimplifier::SimplifyOperation(const OperationId operation, const std::vector<ASTNode*>& operands) const
	{
		auto ruleIt = mSimplifyRules.find(operation);
		if (ruleIt != mSimplifyRules.end())
		{
			return ruleIt->second->Simplify(operands);
		}
		return { false, nullptr };
	}

	const ASTSimplifier& ASTSimplifier::GetDefault()
	{
		static ASTSimplifier instance;

		instance.BindSimplifyRule(OperationId::Addition, new AdditionSimplifyRule(&instance));
		instance.BindSimplifyRule(OperationId::Subtraction, new SubtractionSimplifyRule(&instance));
		instance.BindSimplifyRule(OperationId::Multiplication, new MultiplicationSimplifyRule(&instance));

		return instance;
	}

	OperationSimplifyRule::OperationSimplifyRule(ASTSimplifier* simplifier)
		: mSimplifier(simplifier)
	{
	}

	SimplifyResult OperationSimplifyRule::Simplify(const std::vector<ASTNode*> operands) const
	{
		for (auto rule : mRules)
		{
			std::unique_ptr<SimplifyCheckResult> checkResult = rule->Check(operands);
			if (checkResult && checkResult->mSuccess)
			{
				return rule->Simplify(operands, checkResult);
			}
		}
		return { false, nullptr };
	}
}
