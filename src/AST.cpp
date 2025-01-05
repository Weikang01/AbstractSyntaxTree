#include "AST.h"
#include <stack>


namespace AST
{
	Parser::ParseResult Parser::ExtractNumber(const std::string& expression)
	{
		if (expression.empty())
		{
			return ParseResult(0, ResultType::EmptyExpression);
		}

		size_t index = 0;
		bool hasSign = false;
		bool hasDecimal = false;

		if (expression[index] == '+' || expression[index] == '-')
		{
			hasSign = true;
			++index;
		}

		bool hasDigit = false;

		for (;index < expression.size(); ++index)
		{
			char c = expression[index];

			if (std::isdigit(c))
			{
				hasDigit = true;
			}
			else if (c == '.')
			{
				if (hasDecimal)
				{
					return ParseResult(index, ResultType::InvalidNumberFormat);
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
		else if (hasSign && index == 1)
		{
			return ParseResult(index, ResultType::InvalidNumberFormat);
		}
		else
		{
			return ParseResult(index);
		}
	}

	Rational Parser::ParseNumber(const std::string& expression, const size_t& extractedLength)
	{
		if (extractedLength == 0)
		{
			return Rational();
		}
		std::string numberStr = expression.substr(0, extractedLength);
		return Rational::FromString(numberStr);
	}

	Node* Parser::Parse(const std::string& expression)
	{
		std::stack<Node*> operandStack;
		std::stack<Node*> operatorStack;

		for (size_t i = 0; i < expression.size(); ++i)
		{

		}

		return nullptr;
	}
	void OperatorRegistry::RegisterOperator(std::unique_ptr<Operator> op)
	{
		mOperators.push_back(std::move(op));
	}
	const Operator* OperatorRegistry::GetOperator(const std::string& symbol) const
	{
		for (const auto& op : mOperators)
		{
			if (op->mSymbol == symbol)
			{
				return op.get();
			}
		}
		return nullptr;
	}
}