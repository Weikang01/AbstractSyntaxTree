#pragma once
#include <vector>
#include <unordered_map>
#include "DataTypes/Rational.h"
#include "DataTypes/Operation.h"
#include "DataTypes/Operator.h"

namespace AST
{
	struct ASTNode;
	struct RationalNode;
	struct OperatorNode;
	class ASTSimplifier;

	struct SimplifyCheckResult
	{
		bool mSuccess = false;

		SimplifyCheckResult() = default;
		SimplifyCheckResult(const bool success) : mSuccess(success) {};
		virtual ~SimplifyCheckResult() = default;

		operator bool() const { return mSuccess; }
	};

	struct SimplifyResult
	{
		bool mSuccess = false;
		ASTNode* mResult = nullptr;
		SimplifyResult() = default;
		SimplifyResult(const bool success, ASTNode* result) : mSuccess(success), mResult(result) {}
		virtual ~SimplifyResult() = default;
	};

	struct ISimplifyRule
	{
		int32_t mPriority = 0; // lower is higher priority
		ISimplifyRule(const int32_t priority) : mPriority(priority) {}

		virtual std::unique_ptr<SimplifyCheckResult> Check(const std::vector<ASTNode*> operands) const = 0;
		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands, const std::unique_ptr<SimplifyCheckResult>& checkResult) const = 0;
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

	struct ASTSimplifierSettings
	{
		OperatorRegistry* mOperatorRegistry = OperatorRegistry::GetDefaultRegistry();

		ASTSimplifierSettings() = default;
	};

	struct OperationSimplifyRule
	{
		ASTSimplifier* mSimplifier = nullptr;

		OperationSimplifyRule(ASTSimplifier* simplifier);
		virtual ~OperationSimplifyRule() = default;

		virtual SimplifyResult Simplify(const std::vector<ASTNode*> operands) const;
		OrderedSimplifyRuleList mRules;

		RationalNode* NewRationalNode(const Rational& value) const;
		OperatorNode* NewOperatorNode(const OperationId operationId, const std::vector<ASTNode*>& operands) const;
	};

	class ASTSimplifier
	{
		ASTSimplifierSettings mSettings = {};

		std::unordered_map<OperationId, const OperationSimplifyRule*> mSimplifyRules;

	public:
		void BindSimplifyRule(const OperationId operation, const OperationSimplifyRule* rule);

		ASTNode* Simplify(ASTNode* node) const;

		SimplifyResult SimplifyOperation(const OperationId operation, const std::vector<ASTNode*>& operands) const;

		const ASTSimplifierSettings& GetSettings() const { return mSettings; }

		static const ASTSimplifier& GetDefault();
	};
}