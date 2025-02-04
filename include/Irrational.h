#pragma once
#include "Symbol.h"

namespace AST
{
	enum class IrrationalId : uint8_t
	{
		Invalid = 0,
		Pi,
		E,
		Phi
	};

	struct Irrational : public Symbol
	{
		double mValue;
		IrrationalId mId;
		Irrational(const std::string& symbol, const double value, const IrrationalId& id)
			: Symbol(symbol), mValue(value), mId(id)
		{
		};
	};

	class IrrationalRegistry : public SymbolRegistry
	{
	public:
		static const IrrationalRegistry& GetDefaultRegistry();
	};
}