#pragma once
#include "Symbol.h"

namespace AST
{
	struct Parenthesis : public Symbol
	{
		bool mIsOpen = false;
		Parenthesis* mOpposite = nullptr;

		Parenthesis(const std::string& symbol, bool isOpen, Parenthesis* opposite = nullptr)
			: Symbol(symbol), mIsOpen(isOpen), mOpposite(opposite)
		{}

		virtual ~Parenthesis() = default;
	};

	class ParenthesisRegistry : public SymbolRegistry
	{
	public:
		static const ParenthesisRegistry& GetDefaultRegistry();
	};
}