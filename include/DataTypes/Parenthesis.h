#pragma once
#include "DataTypes/Symbol.h"

namespace AST
{
	struct Parenthesis : public Symbol
	{
		bool mIsOpen = false;
		Parenthesis* mOpposite = nullptr;

		Parenthesis(const std::string& symbol, bool isOpen, Parenthesis* opposite = nullptr)
			: Symbol(symbol), mIsOpen(isOpen), mOpposite(opposite)
		{
		}

		virtual ~Parenthesis() = default;

		bool IsOpposite(const Parenthesis* other) const;
	};

	class ParenthesisRegistry : public SymbolRegistry
	{
	public:
		static const ParenthesisRegistry& GetDefaultRegistry();
	};
}