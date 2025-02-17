#include "ASTSimplifier.h"

namespace AST
{
	struct AdditionSimplifyRule : public OperationSimplifyRule
	{
		ASTNode* CombineCommonTerms(const ASTNode* left, const ASTNode* right, const ASTNode* common);
		AdditionSimplifyRule(ASTSimplifier* simplifier);
	};

	struct SubtractionSimplifyRule : public OperationSimplifyRule
	{
		SubtractionSimplifyRule(ASTSimplifier* simplifier);
	};

	struct MultiplicationSimplifyRule : public OperationSimplifyRule
	{
		MultiplicationSimplifyRule(ASTSimplifier* simplifier);
	};
}