#pragma once
#include <iostream>
#include <vector>
#include "Rational.h"

namespace AST
{
	struct Node
	{
	public:
		virtual ~Node() = default;
	};

	enum class OperatorType : uint8_t
	{
		Unary, // Unary operators have only one operand
		Binary, // Binary operators have two operands
	};

	enum class Associativity : uint8_t
	{
		LeftToRight,
		RightToLeft,
	};

	struct Operator
	{
		OperatorType mType;
		Associativity mAssociativity;
		uint16_t mPrecedence;
		std::string mSymbol;

		Operator(OperatorType type, Associativity associativity, uint16_t precedence, const std::string& symbol)
			: mType(type), mAssociativity(associativity), mPrecedence(precedence), mSymbol(symbol) {};

		virtual ~Operator() = default;
	};

	class OperatorRegistry
	{
	private:
		std::vector<std::unique_ptr<Operator>> mOperators;

	public:
		OperatorRegistry() = default;
		void RegisterOperator(std::unique_ptr<Operator> op);
		const Operator* GetOperator(const std::string& symbol) const;
	};
	
	class Parser
	{
	public:
		enum class ResultType : uint8_t
		{
			NoError,
			InvalidCharacter,
			InvalidNumberFormat,
			InvalidOperator,
			InvalidParenthesis,
			InvalidExpression,
			EmptyExpression,
		};

	private:
		struct ParseResult
		{
			size_t extractedLength = 0;
			size_t errorPos = -1;
			ResultType errorType = ResultType::NoError;

			ParseResult(size_t extractedLength) : extractedLength(extractedLength) {}
			ParseResult(size_t errorPos, ResultType errorType) : errorPos(errorPos), errorType(errorType) {}

			operator bool() const { return errorType == ResultType::NoError; }
			bool HasError() const { return errorType != ResultType::NoError; }

			friend std::ostream& operator<<(std::ostream& os, const ParseResult& result)
			{
				if (result.HasError())
				{
					os << "Error at position " << result.errorPos << ": ";
					switch (result.errorType)
					{
					case ResultType::InvalidCharacter:
						os << "Invalid character";
						break;
					case ResultType::InvalidNumberFormat:
						os << "Invalid number format";
						break;
					case ResultType::InvalidOperator:
						os << "Invalid operator";
						break;
					case ResultType::InvalidParenthesis:
						os << "Invalid parenthesis";
						break;
					case ResultType::InvalidExpression:
						os << "Invalid expression";
						break;
					case ResultType::EmptyExpression:
						os << "Empty expression";
						break;
					default:
						os << "Unknown error";
						break;
					}
				}
				else
				{
					os << "No error";
				}
				return os;
			}
		};

		/// <summary>
		/// This function extracts a valid number from the expression.
		/// </summary>
		/// <param name="expression">expression to extract the number from</param>
		/// <returns>ParseResult object containing the extracted length and error information</returns>
		static ParseResult ExtractNumber(const std::string& expression);

		static Rational ParseNumber(const std::string& expression, const size_t& extractedLength);

	public:
		static Node* Parse(const std::string& expression);
	};
}