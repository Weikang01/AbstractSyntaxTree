#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <functional>

namespace AST
{
	struct Symbol
	{
		std::string mSymbol;
		Symbol(const std::string& symbol) : mSymbol(symbol) {};
		virtual ~Symbol() = default;

		virtual bool operator==(const Symbol& other) const;
		bool operator!=(const Symbol& other) const { return !(*this == other); }
	};

	struct SymbolSearchNode
	{
		std::vector<std::shared_ptr<Symbol>> mSymbols = {};
		std::map<char, std::shared_ptr<SymbolSearchNode>> mChildren = {};

		SymbolSearchNode() = default;
		~SymbolSearchNode();

		void Add(std::shared_ptr<Symbol> symbol, const size_t index = 0);
		void Remove(const std::string& symbol, const size_t index = 0);

		// Function to find the first matched symbol based on user-defined logic
		std::shared_ptr<Symbol> FindFirstMatch(const std::function<bool(const std::shared_ptr<Symbol>&)>& predicate) const
		{
			for (const auto& symbol : mSymbols)
			{
				if (predicate(symbol))
				{
					return symbol;
				}
			}
			return nullptr;
		}
	};

	class SymbolRegistry
	{
	private:
		std::shared_ptr<SymbolSearchNode> mRoot = std::make_shared<SymbolSearchNode>();
	public:
		SymbolRegistry() = default;
		void RegisterSymbol(std::shared_ptr<Symbol> symbol);
		void UnregisterSymbol(const std::string& symbol);

		const std::shared_ptr<SymbolSearchNode>& GetRoot() const { return mRoot; }

		std::shared_ptr<Symbol> GetSymbol(const std::string& symbol, const std::function<bool(const std::shared_ptr<Symbol>&)>& findFirstMatch) const;
	};
}