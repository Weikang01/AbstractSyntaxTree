#pragma once
#include "ASTNode.h"

namespace AST
{
	class IOperation
	{
		OperatorNode* mOperatorNode = nullptr;
	public:
		virtual ~IOperation(const OperatorNode* operatorNode) = default;
		
		virtual ASTNode* SimplifyRationals() const = 0;
		virtual ASTNode* SimplifyRationalsAndIrrationals() const = 0;
	};
}