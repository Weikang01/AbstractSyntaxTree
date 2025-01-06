#pragma once
#include <iostream>
#include <map>

namespace AST
{
	struct Symbol
	{
		std::string mSymbol;
		Symbol(const std::string& symbol) : mSymbol(symbol) {};
		virtual ~Symbol() = default;
	};

	struct SymbolSearchNode
	{
		std::shared_ptr<Symbol> mSymbol = nullptr;
		std::map<char, std::shared_ptr<SymbolSearchNode>> mChildren = {};

		SymbolSearchNode() = default;
		~SymbolSearchNode();

		void Add(std::shared_ptr<Symbol> symbol, const size_t index = 0);
		void Remove(const std::string& symbol, const size_t index = 0);
	};

	class SymbolRegistry
	{
	private:
		std::shared_ptr<SymbolSearchNode> mRoot = std::make_shared<SymbolSearchNode>();
	public:
		SymbolRegistry() = default;
		void RegisterSymbol(std::shared_ptr<Symbol> symbol);
		void UnregisterOperator(const std::string& symbol);

		const std::shared_ptr<SymbolSearchNode>& GetRoot() const { return mRoot; }
	};
}