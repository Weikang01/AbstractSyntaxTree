#include "AST.h"

namespace AST
{
	Parser::ParseResult Parser::ExtractRational(const std::string& expression, const size_t& offset)
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

	Rational Parser::ParseRational(const std::string& expression, const size_t& offset, const size_t& mExtractedLength)
	{
		if (mExtractedLength == 0)
		{
			return Rational();
		}
		std::string numberStr = expression.substr(offset, mExtractedLength);
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

	void Parser::ConstructOperatorTree(std::deque<Node*>& operandStack, std::deque<Node*>& operatorStack, OperatorNode* topOperator)
	{
		if (topOperator->mOperator->mType == OperatorType::Binary)
		{
			Node* right = operandStack.back();
			operandStack.pop_back();
			Node* left = operandStack.back();
			operandStack.pop_back();
			operatorStack.pop_back();

			topOperator->mOperands.push_back(left);
			topOperator->mOperands.push_back(right);

			operandStack.push_back(topOperator);
		}
		else if (topOperator->mOperator->mType == OperatorType::Unary)
		{
			Node* operand = operandStack.back();
			operandStack.pop_back();
			operatorStack.pop_back();
			topOperator->mOperands.push_back(operand);
			operandStack.push_back(topOperator);
		}
		else if (topOperator->mOperator->mType == OperatorType::FunctionSingular)
		{
			Node* operand = operandStack.back();
			operandStack.pop_back();
			operatorStack.pop_back();
			topOperator->mOperands.push_back(operand);
			operandStack.push_back(topOperator);
		}
		else if (topOperator->mOperator->mType == OperatorType::FunctionDual)
		{
			Node* right = operandStack.back();
			operandStack.pop_back();
			Node* left = operandStack.back();
			operandStack.pop_back();
			operatorStack.pop_back();
			topOperator->mOperands.push_back(left);
			topOperator->mOperands.push_back(right);
			operandStack.push_back(topOperator);
		}
	}

	Node* Parser::Parse(const std::string& expression)
	{
		std::deque<Node*> operandStack;
		std::deque<Node*> operatorStack;

		Node::NodeType lastNodeType = Node::NodeType::Unknown;

		size_t offset = 0;
		while (offset < expression.size())
		{
			if (expression[offset] == ' ')
			{
				++offset;
				continue;
			}

			ParseResult result;

			// TODO: Handle parentheses

			if (lastNodeType != Node::NodeType::Rational)
			{
				result = ExtractRational(expression, offset);
				if (!result.HasError())
				{
					operandStack.push_back(new RationalNode(ParseRational(expression, offset, result.mExtractedLength)));
					offset += result.mExtractedLength;
					lastNodeType = Node::NodeType::Rational;
					continue;
				}
			}

			result = ExtractIrrational(expression, offset);
			if (!result.HasError())
			{
				operandStack.push_back(new IrrationalNode(result.mSymbol));
				offset += result.mExtractedLength;
				lastNodeType = Node::NodeType::Irrational;
				continue;
			}

			result = ExtractOperator(expression, offset);
			if (!result.HasError())
			{
				const Operator* op = dynamic_cast<const Operator*>(result.mSymbol);
				while (!operatorStack.empty()) // TODO: Or closing parenthesis
				{
					OperatorNode* operatorNode = dynamic_cast<OperatorNode*>(operatorStack.back());
					if (operatorNode->mOperator->mPrecedence > op->mPrecedence)
					{
						ConstructOperatorTree(operandStack, operatorStack, operatorNode);
					}
					else
					{
						break;
					}
				}

				operatorStack.push_back(new OperatorNode(op));
				offset += result.mExtractedLength;
				lastNodeType = Node::NodeType::Operator;
				continue;
			}
		}

		while (!operatorStack.empty())
		{
			OperatorNode* operatorNode = dynamic_cast<OperatorNode*>(operatorStack.back());
			ConstructOperatorTree(operandStack, operatorStack, operatorNode);
		}

		if (operandStack.size() != 1)
		{
			// TODO: Handle dangling operands, report error
			return nullptr;
		}

		return operandStack.back();
	}
}