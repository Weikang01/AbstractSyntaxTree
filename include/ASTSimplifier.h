#pragma once
#include <vector>
#include <unordered_map>
#include "DataTypes/Operation.h"

namespace AST
{
	struct ASTNode;

	struct SimplifyResult
	{
		bool mSuccess = false;
		ASTNode* mResult = nullptr;
		SimplifyResult() = default;
		SimplifyResult(const bool success, ASTNode* result) : mSuccess(success), mResult(result) {}
	};

	struct ISimplifyRule
	{
		int32_t mPriority = 0; // lower is higher priority
		ISimplifyRule(const int32_t priority) : mPriority(priority) {}

		virtual bool Check(const std::vector<ASTNode*> operands) const = 0;
		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands) const = 0;
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

	struct OperationSimplifyRule
	{
		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands) const;
		OrderedSimplifyRuleList mRules;
	};

	class ASTSimplifier
	{
		std::unordered_map<OperationId, const OperationSimplifyRule*> mSimplifyRules;

	public:
		void BindSimplifyRule(const OperationId operation, const OperationSimplifyRule* rule);

		ASTNode* Simplify(ASTNode* node) const;

		static const ASTSimplifier& GetDefault();
	};
}