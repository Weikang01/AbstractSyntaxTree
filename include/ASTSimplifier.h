#pragma once
#include <vector>
#include <unordered_map>
#include "Operation.h"

namespace AST
{
	struct ASTNode;

	struct ISimplifyRule
	{
		int32_t mPriority = 0; // lower is higher priority
		virtual ASTNode* Simplify(const std::vector<ASTNode*> operands, const int32_t priority) const = 0;
	};

	struct OrderedSimplifyRuleList
	{
	private:
		std::vector<ISimplifyRule*> mRules = {};

	public:
		void AddRule(ISimplifyRule* rule);

		using Iterator = std::vector<ISimplifyRule*>::iterator;
		using ConstIterator = std::vector<ISimplifyRule*>::const_iterator;

		Iterator begin() { return mRules.begin(); }
		Iterator end() { return mRules.end(); }
		ConstIterator begin() const { return mRules.begin(); }
		ConstIterator end() const { return mRules.end(); }
	};

	struct IOperationSimplifyRule
	{
		virtual ASTNode* Simplify(const std::vector<ASTNode*> operands) const = 0;
		OrderedSimplifyRuleList mRules;
	};

	class ASTSimplifier
	{
		std::unordered_map<OperationId, const IOperationSimplifyRule*> mSimplifyRules;

	public:
		void BindSimplifyRule(const OperationId operation, const IOperationSimplifyRule* rule);

		ASTNode* Simplify(ASTNode* node) const;
	};
}