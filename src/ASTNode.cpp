#include "ASTNode.h"

namespace AST
{
	OperatorNode::~OperatorNode()
	{
		for (auto& operand : mOperands)
		{
			delete operand;
		}
	}
}