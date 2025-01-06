#pragma once
#include "Symbol.h"

namespace AST
{
	struct Irrational : public Symbol
	{
		double mValue;
		Irrational(const std::string& symbol, const double value)
			: Symbol(symbol), mValue(value)
		{};
	};

	class IrrationalRegistry : public SymbolRegistry
	{
	public:
		static const IrrationalRegistry& GetDefaultRegistry();
	};
}