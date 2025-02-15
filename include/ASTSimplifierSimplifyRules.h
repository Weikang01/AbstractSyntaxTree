#include "ASTSimplifier.h"

namespace AST
{
	struct AdditionSimplifyRule : public OperationSimplifyRule
	{
		AdditionSimplifyRule();
	};

	struct SubtractionSimplifyRule : public OperationSimplifyRule
	{
		SubtractionSimplifyRule();
	};
}