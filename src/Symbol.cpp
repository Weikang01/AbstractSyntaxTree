#include "Symbol.h"

namespace AST
{
	SymbolSearchNode::~SymbolSearchNode()
	{
		for (auto& child : mChildren)
		{
			child.second.reset();
		}
	}
	void SymbolSearchNode::Add(std::shared_ptr<Symbol> symbol, const size_t index)
	{
		if (index == symbol->mSymbol.size())
		{
			mSymbol = symbol;
			return;
		}
		if (mChildren.find(symbol->mSymbol[index]) == mChildren.end())
		{
			mChildren[symbol->mSymbol[index]] = std::make_shared<SymbolSearchNode>();
		}
		mChildren[symbol->mSymbol[index]]->Add(symbol, index + 1);
	}

	void SymbolSearchNode::Remove(const std::string& symbol, const size_t index)
	{
		if (index == symbol.size() - 1)
		{
			mChildren.erase(symbol[index]);
			return;
		}
		if (mChildren.find(symbol[index]) == mChildren.end())
		{
			return;
		}
		mChildren[symbol[index]]->Remove(symbol, index + 1);
	}
	
	void SymbolRegistry::RegisterSymbol(std::shared_ptr<Symbol> symbol)
	{
		mRoot->Add(symbol);
	}

	void SymbolRegistry::UnregisterOperator(const std::string& symbol)
	{
		mRoot->Remove(symbol);
	}
}
