#include "DataTypes/Symbol.h"

namespace AST
{
	bool Symbol::operator==(const Symbol& other) const
	{
		return mSymbol == other.mSymbol;
	}

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
			mSymbols.push_back(symbol);
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

	void SymbolRegistry::UnregisterSymbol(const std::string& symbol)
	{
		mRoot->Remove(symbol);
	}

	std::shared_ptr<Symbol> SymbolRegistry::GetSymbol(const std::string& symbol, const std::function<bool(const std::shared_ptr<Symbol>&)>& findFirstMatch) const
	{
		auto node = mRoot;
		for (size_t i = 0; i < symbol.size(); ++i)
		{
			auto it = node->mChildren.find(symbol[i]);
			if (it == node->mChildren.end())
			{
				return nullptr; // Symbol not found
			}
			node = it->second;
		}

		return node->FindFirstMatch(findFirstMatch);


	}
}
