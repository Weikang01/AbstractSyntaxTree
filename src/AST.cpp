#include "AST.h"
#include <stack>


namespace AST
{
	Parser::ParseResult Parser::ExtractNumber(const std::string& expression, const size_t& offset)
	{
		if (expression.size() <= offset)
		{
			return ParseResult(0, ResultType::EmptyExpression);
		}

		size_t extractedLength = 0;
		bool hasSign = false;
		bool hasDecimal = false;

		if (expression[offset] == '+' || expression[offset] == '-')
		{
			hasSign = true;
			++extractedLength;
		}

		bool hasDigit = false;

		for (; offset + extractedLength < expression.size(); ++extractedLength)
		{
			char c = expression[offset + extractedLength];

			if (std::isdigit(c))
			{
				hasDigit = true;
			}
			else if (c == '.')
			{
				if (hasDecimal)
				{
					return ParseResult(extractedLength, ResultType::InvalidNumberFormat);
				}
				hasDecimal = true;
			}
			else
			{
				break;
			}
		}

		if (!hasDigit)
		{
			return ParseResult(0, ResultType::InvalidNumberFormat);
		}
		else if (hasSign && extractedLength == 1)
		{
			return ParseResult(extractedLength, ResultType::InvalidNumberFormat);
		}
		else
		{
			return ParseResult(extractedLength);
		}
	}

	Rational Parser::ParseNumber(const std::string& expression, const size_t& mExtractedLength)
	{
		if (mExtractedLength == 0)
		{
			return Rational();
		}
		std::string numberStr = expression.substr(0, mExtractedLength);
		return Rational::FromString(numberStr);
	}

	Parser::ParseResult Parser::ExtractSymbol(const SymbolRegistry& symbolRegistry, const std::string& expression, const size_t& offset)
	{
		if (expression.empty())
		{
			return ParseResult(0, ResultType::EmptyExpression);
		}
		
		std::shared_ptr<SymbolSearchNode> node = symbolRegistry.GetRoot();
		std::shared_ptr<Symbol> longestMatchSymbol = nullptr;
		size_t extractedLength = 0;
		size_t searchLength = 0;

		while (node)
		{
			if (offset + searchLength >= expression.size())
			{
				break;
			}
			char c = expression[offset + searchLength];
			if (node->mChildren.find(c) == node->mChildren.end())
			{
				break;
			}
			node = node->mChildren[c];
			if (node->mSymbol)
			{
				longestMatchSymbol = node->mSymbol;
				extractedLength = searchLength + 1;
			}
			++searchLength;
		}

		if (longestMatchSymbol)
		{
			return ParseResult(extractedLength, longestMatchSymbol.get());
		}
		else
		{
			return ParseResult(0, ResultType::InvalidOperator);
		}
	}

	Parser::ParseResult Parser::ExtractOperator(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mOperatorRegistry, expression, offset);
	}

	Parser::ParseResult Parser::ExtractIrrational(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mIrrationalRegistry, expression, offset);
	}

	Node* Parser::Parse(const std::string& expression)
	{
		std::stack<Node*> operandStack;
		std::stack<Node*> operatorStack;

		size_t offset = 0;
		while (offset < expression.size())
		{
			// TODO: Implement parsing logic
		}

		return nullptr;
	}
}