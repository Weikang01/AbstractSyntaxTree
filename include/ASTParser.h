#pragma once
#include <deque>

#include "Rational.h"
#include "Symbol.h"
#include "Operator.h"
#include "Irrational.h"
#include "Parenthesis.h"
#include "ASTNode.h"

#include <functional>

namespace AST
{
	struct ASTNode;
	struct OperatorNode;

	class ASTParser
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
		ParenthesisRegistry mParenthesisRegistry;
		bool mMatchExactParenthesis = true;

		struct ParseResult
		{
			size_t mExtractedLength = 0;
			size_t mErrorPos = -1;
			ResultType mErrorType = ResultType::NoError;
			const Symbol* mSymbol = nullptr;

			ParseResult() = default;
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

		Rational ParseRational(const std::string& expression, const size_t& offset, const size_t& mExtractedLength);

		ParseResult ExtractSymbol(const SymbolRegistry& symbolRegistry, const std::string& expression, const std::function<bool(const std::shared_ptr<Symbol>&)>& findFirstMatch, const size_t& offset = 0);

		ParseResult ExtractOperator(const std::string& expression, const ASTNode::NodeType& lastNodeType, const size_t& offset = 0);

		ParseResult ExtractIrrational(const std::string& expression, const size_t& offset = 0);

		ParseResult ExtractParenthesis(const std::string& expression, const size_t& offset = 0);

		void ReduceOperator(std::deque<ASTNode*>& operandStack, std::deque<ASTNode*>& operatorStack, OperatorNode* topOperator = nullptr);

	public:
		ASTParser(const OperatorRegistry& operatorRegistry = OperatorRegistry::GetDefaultRegistry(),
			const IrrationalRegistry& irrationalRegistry = IrrationalRegistry::GetDefaultRegistry(),
			const ParenthesisRegistry& parenthesisRegistry = ParenthesisRegistry::GetDefaultRegistry())
			:
			mOperatorRegistry(operatorRegistry),
			mIrrationalRegistry(irrationalRegistry),
			mParenthesisRegistry(parenthesisRegistry)
		{
		}
		ASTParser(ASTParser&&) = default;
		ASTParser(const ASTParser&) = default;
		ASTParser& operator=(ASTParser&&) = default;
		ASTParser& operator=(const ASTParser&) = default;
		virtual ~ASTParser() = default;

		ASTNode* Parse(const std::string& expression);
	};
}