#include "Parenthesis.h"

namespace AST
{
	static void RegisterParenthesisPairs(ParenthesisRegistry& registry, const Parenthesis* left, const Parenthesis* right)
	{
		registry.RegisterSymbol(std::make_shared<Parenthesis>(left->mSymbol, true, const_cast<Parenthesis*>(right)));
		registry.RegisterSymbol(std::make_shared<Parenthesis>(right->mSymbol, false, const_cast<Parenthesis*>(left)));
	}

	const ParenthesisRegistry& ParenthesisRegistry::GetDefaultRegistry()
	{
		static ParenthesisRegistry defaultRegistry;
		// Register symbols
		RegisterParenthesisPairs(defaultRegistry, new Parenthesis("(", true), new Parenthesis(")", false));
		RegisterParenthesisPairs(defaultRegistry, new Parenthesis("[", true), new Parenthesis("]", false));
		RegisterParenthesisPairs(defaultRegistry, new Parenthesis("{", true), new Parenthesis("}", false));
		
		return defaultRegistry;
	}
}