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
		std::vector<Operator*> mOperators = {};

	public:
		OperatorRegistry() = default;
		void RegisterOperator(Operator* mOperator);
		const Operator* GetOperator(const std::string& symbol) const;

		const std::vector<Operator*>& GetOperators() const { return mOperators; }

		static const OperatorRegistry& GetDefaultRegistry();
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
#ifndef UNIT_TEST
	private:
#endif
		OperatorRegistry mOperatorRegistry;

		struct ParseResult
		{
			size_t mExtractedLength = 0;
			size_t mErrorPos = -1;
			ResultType mErrorType = ResultType::NoError;
			const Operator* mOperator = nullptr;

			ParseResult(size_t mExtractedLength, const Operator* mOperator = nullptr) : mExtractedLength(mExtractedLength), mOperator(mOperator) {}
			ParseResult(size_t mErrorPos, ResultType mErrorType) : mErrorPos(mErrorPos), mErrorType(mErrorType) {}

			operator bool() const { return mErrorType == ResultType::NoError; }
			bool HasError() const { return mErrorType != ResultType::NoError; }

			friend std::ostream& operator<<(std::ostream& os, const ParseResult& result)
			{
				if (result.HasError())
				{
					os << "Error at position " << result.mErrorPos << ": ";
					switch (result.mErrorType)
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
		ParseResult ExtractNumber(const std::string& expression);

		Rational ParseNumber(const std::string& expression, const size_t& mExtractedLength);

		ParseResult ExtractOperator(const std::string& expression);

	public:
		Parser(const OperatorRegistry& operatorRegistry = OperatorRegistry::GetDefaultRegistry()) : mOperatorRegistry(operatorRegistry) {}

		Node* Parse(const std::string& expression);
	};
}