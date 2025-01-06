#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Symbol.h"
#include "Operator.h"
#include "Irrational.h"
#include "Rational.h"

namespace AST
{
	struct Node
	{
	public:
		enum class NodeType : uint8_t
		{
			Unknown,
			Rational,
			Irrational,
			Operator,
		};

		NodeType mType = NodeType::Unknown;

		Node(const NodeType type) : mType(type) {}

		virtual ~Node() = default;
	};

	struct RationalNode : public Node
	{
		Rational mValue;

		RationalNode(const Rational& value)
			: mValue(value), Node(NodeType::Rational) {}
	};

	struct IrrationalNode : public Node
	{
		const std::weak_ptr<Irrational> mIrrational;
		IrrationalNode(const std::shared_ptr<Irrational>& irrational)
			: mIrrational(irrational), Node(NodeType::Irrational) {}
	};

	struct OperatorNode : public Node
	{
		// support for unary, binary, function singular, function dual, function multiple
		const std::weak_ptr<Operator> mOperator;
		std::vector<std::shared_ptr<Node>> mOperands;
		OperatorNode(const std::shared_ptr<Operator>& op)
			: mOperator(op), Node(NodeType::Operator) {}
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
		IrrationalRegistry mIrrationalRegistry;

		struct ParseResult
		{
			size_t mExtractedLength = 0;
			size_t mErrorPos = -1;
			ResultType mErrorType = ResultType::NoError;
			const Symbol* mSymbol = nullptr;

			ParseResult(const size_t extractedLength, const Symbol* symbol = nullptr) : mExtractedLength(extractedLength), mSymbol(symbol) {}
			ParseResult(const size_t errorPos, const ResultType errorType) : mErrorPos(errorPos), mErrorType(errorType) {}

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
		ParseResult ExtractRational(const std::string& expression, const size_t& offset = 0);

		Rational ParseNumber(const std::string& expression, const size_t& mExtractedLength);

		ParseResult ExtractSymbol(const SymbolRegistry& symbolRegistry, const std::string& expression, const size_t& offset = 0);

		ParseResult ExtractOperator(const std::string& expression, const size_t& offset = 0);

		ParseResult ExtractIrrational(const std::string& expression, const size_t& offset = 0);

	public:
		Parser(const OperatorRegistry& operatorRegistry = OperatorRegistry::GetDefaultRegistry(),
			const IrrationalRegistry& irrationalRegistry = IrrationalRegistry::GetDefaultRegistry())
			: mOperatorRegistry(operatorRegistry), mIrrationalRegistry(irrationalRegistry)
		{}
		Parser(Parser&&) = default;
		Parser(const Parser&) = default;
		Parser& operator=(Parser&&) = default;
		Parser& operator=(const Parser&) = default;
		virtual ~Parser() = default;

		Node* Parse(const std::string& expression);
	};
}