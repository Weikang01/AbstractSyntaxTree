#include "DataTypes/Parenthesis.h"

namespace AST
{
	bool Parenthesis::IsOpposite(const Parenthesis* other) const
	{
		return mOpposite && mOpposite->mSymbol == other->mSymbol;
	}

	static void RegisterParenthesisPairs(ParenthesisRegistry* registry, const Parenthesis* left, const Parenthesis* right)
	{
		registry->RegisterSymbol(std::make_shared<Parenthesis>(left->mSymbol, true, const_cast<Parenthesis*>(right)));
		registry->RegisterSymbol(std::make_shared<Parenthesis>(right->mSymbol, false, const_cast<Parenthesis*>(left)));
	}

	ParenthesisRegistry* ParenthesisRegistry::GetDefaultRegistry()
	{
		static ParenthesisRegistry* defaultRegistry = new ParenthesisRegistry();
		static bool initialized = false;

		if (initialized)
		{
			return defaultRegistry;
		}

		initialized = true;
		// Register symbols
		RegisterParenthesisPairs(defaultRegistry, new Parenthesis("(", true), new Parenthesis(")", false));
		RegisterParenthesisPairs(defaultRegistry, new Parenthesis("[", true), new Parenthesis("]", false));
		RegisterParenthesisPairs(defaultRegistry, new Parenthesis("{", true), new Parenthesis("}", false));

		return defaultRegistry;
	}
}