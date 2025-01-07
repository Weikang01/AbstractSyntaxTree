#include "ASTParser.h"
#include "ASTNode.h"

namespace AST
{
	ASTParser::ParseResult ASTParser::ExtractRational(const std::string& expression, const size_t& offset)
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

	Rational ASTParser::ParseRational(const std::string& expression, const size_t& offset, const size_t& mExtractedLength)
	{
		if (mExtractedLength == 0)
		{
			return Rational();
		}
		std::string numberStr = expression.substr(offset, mExtractedLength);
		return Rational::FromString(numberStr);
	}

	ASTParser::ParseResult ASTParser::ExtractSymbol(const SymbolRegistry& symbolRegistry, const std::string& expression, const size_t& offset)
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

	ASTParser::ParseResult ASTParser::ExtractOperator(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mOperatorRegistry, expression, offset);
	}

	ASTParser::ParseResult ASTParser::ExtractIrrational(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mIrrationalRegistry, expression, offset);
	}

	ASTParser::ParseResult ASTParser::ExtractParenthesis(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mParenthesisRegistry, expression, offset);
	}

	void ASTParser::ReduceOperator(std::deque<ASTNode*>& operandStack, std::deque<ASTNode*>& operatorStack, OperatorNode* topOperator)
	{
		if (topOperator->mOperator->mType == OperatorType::Binary)
		{
			ASTNode* right = operandStack.back();
			operandStack.pop_back();
			ASTNode* left = operandStack.back();
			operandStack.pop_back();
			operatorStack.pop_back();

			topOperator->mOperands.push_back(left);
			topOperator->mOperands.push_back(right);

			operandStack.push_back(topOperator);
		}
		else if (topOperator->mOperator->mType == OperatorType::Unary)
		{
			ASTNode* operand = operandStack.back();
			operandStack.pop_back();
			operatorStack.pop_back();
			topOperator->mOperands.push_back(operand);
			operandStack.push_back(topOperator);
		}
		else if (topOperator->mOperator->mType == OperatorType::FunctionSingular)
		{
			ASTNode* operand = operandStack.back();
			operandStack.pop_back();
			operatorStack.pop_back();
			topOperator->mOperands.push_back(operand);
			operandStack.push_back(topOperator);
		}
		else if (topOperator->mOperator->mType == OperatorType::FunctionDual)
		{
			ASTNode* right = operandStack.back();
			operandStack.pop_back();
			ASTNode* left = operandStack.back();
			operandStack.pop_back();
			operatorStack.pop_back();
			topOperator->mOperands.push_back(left);
			topOperator->mOperands.push_back(right);
			operandStack.push_back(topOperator);
		}
	}

	ASTNode* ASTParser::Parse(const std::string& expression)
	{
		std::deque<ASTNode*> operandStack;
		std::deque<ASTNode*> operatorStack;

		ASTNode::NodeType lastNodeType = ASTNode::NodeType::Unknown;

		size_t offset = 0;
		while (offset < expression.size())
		{
			if (expression[offset] == ' ')
			{
				++offset;
				continue;
			}

			ParseResult result = ExtractParenthesis(expression, offset);
			if (!result.HasError())
			{
				ParenthesisNode* parenthesisNode = new ParenthesisNode(result.mSymbol);
				if (parenthesisNode->mParenthesis->mIsOpen)
				{
					operatorStack.push_back(parenthesisNode);
					offset += result.mExtractedLength;
					lastNodeType = ASTNode::NodeType::Parenthesis;
					continue;
				}
				else
				{
					// TODO: Handle closing parenthesis
					while (!operatorStack.empty())
					{
						ASTNode* top = operatorStack.back();
						if (top->mType == ASTNode::NodeType::Parenthesis)
						{
							operatorStack.pop_back();
							break;
						}
						else if (top->mType == ASTNode::NodeType::Operator)
						{
							ReduceOperator(operandStack, operatorStack, dynamic_cast<OperatorNode*>(top));
						}
					}
				}
			}

			if (lastNodeType != ASTNode::NodeType::Rational)
			{
				result = ExtractRational(expression, offset);
				if (!result.HasError())
				{
					operandStack.push_back(new RationalNode(ParseRational(expression, offset, result.mExtractedLength)));
					offset += result.mExtractedLength;
					lastNodeType = ASTNode::NodeType::Rational;
					continue;
				}
			}

			result = ExtractIrrational(expression, offset);
			if (!result.HasError())
			{
				operandStack.push_back(new IrrationalNode(result.mSymbol));
				offset += result.mExtractedLength;
				lastNodeType = ASTNode::NodeType::Irrational;
				continue;
			}

			result = ExtractOperator(expression, offset);
			if (!result.HasError())
			{
				const Operator* op = dynamic_cast<const Operator*>(result.mSymbol);
				while (!operatorStack.empty())
				{
					ASTNode* top = operatorStack.back();
					if (top->mType == ASTNode::NodeType::Parenthesis)
					{
						break;
					}
					else if (top->mType == ASTNode::NodeType::Operator)
					{
						OperatorNode* operatorNode = dynamic_cast<OperatorNode*>(top);
						if (operatorNode->mOperator->mPrecedence > op->mPrecedence)
						{
							ReduceOperator(operandStack, operatorStack, operatorNode);
						}
						else
						{
							break;
						}
					}
				}

				operatorStack.push_back(new OperatorNode(op));
				offset += result.mExtractedLength;
				lastNodeType = ASTNode::NodeType::Operator;
				continue;
			}
		}

		while (!operatorStack.empty())
		{
			OperatorNode* operatorNode = dynamic_cast<OperatorNode*>(operatorStack.back());
			ReduceOperator(operandStack, operatorStack, operatorNode);
		}

		if (operandStack.size() != 1)
		{
			// TODO: Handle dangling operands, report error
			return nullptr;
		}

		return operandStack.back();
	}
}