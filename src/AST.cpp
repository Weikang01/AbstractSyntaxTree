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

	Rational Parser::ParseNumber(const std::string& expression, const size_t& mExtractedLength)
	{
		if (mExtractedLength == 0)
		{
			return Rational();
		}
		std::string numberStr = expression.substr(0, mExtractedLength);
		return Rational::FromString(numberStr);
	}

	Parser::ParseResult Parser::ExtractOperator(const std::string& expression)
	{
		if (expression.empty())
		{
			return ParseResult(0, ResultType::EmptyExpression);
		}
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
	void OperatorRegistry::RegisterOperator(Operator* mOperator)
	{
		mOperators.push_back(std::move(mOperator));
	}
	const Operator* OperatorRegistry::GetOperator(const std::string& symbol) const
	{
		for (const auto& mOperator : mOperators)
		{
			if (mOperator->mSymbol == symbol)
			{
				return mOperator;
			}
		}
		return nullptr;
	}
	const OperatorRegistry& OperatorRegistry::GetDefaultRegistry()
	{
		static OperatorRegistry defaultRegistry;
		// Register operators
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Binary, Associativity::LeftToRight, 1, "+"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Binary, Associativity::LeftToRight, 1, "-"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Binary, Associativity::LeftToRight, 2, "*"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Binary, Associativity::LeftToRight, 2, "/"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 3, "-"));

		// pow operator
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Binary, Associativity::RightToLeft, 4, "^"));

		// root operator
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Binary, Associativity::RightToLeft, 4, "root"));

		// trigonometric functions
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "sin"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "cos"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "tan"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "asin"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "acos"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "atan"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "sinh"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "cosh"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "tanh"));

		// logarithmic functions
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "ln"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "log"));
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 5, "log_"));

		// factorial operator
		defaultRegistry.RegisterOperator(new Operator(OperatorType::Unary, Associativity::RightToLeft, 6, "!"));

		return defaultRegistry;
	}
}