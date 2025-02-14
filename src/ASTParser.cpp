#include "ASTParser.h"

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

	ASTParser::ParseResult ASTParser::ExtractSymbol(const SymbolRegistry& symbolRegistry, const std::string& expression, const std::function<bool(const std::shared_ptr<Symbol>&)>& findFirstMatch, const size_t& offset)
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
			if (node->mSymbols.size())
			{
				longestMatchSymbol = node->FindFirstMatch(findFirstMatch);
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

	ASTParser::ParseResult ASTParser::ExtractOperator(const std::string& expression, const ASTNode::NodeType& lastNodeType, const size_t& offset)
	{
		return ExtractSymbol(mOperatorRegistry, expression, [&](const std::shared_ptr<Symbol>& symbol)
			{
				const Operator* op = dynamic_cast<const Operator*>(symbol.get());
				if (op->mType == OperatorType::Binary && (lastNodeType == ASTNode::NodeType::Operator || lastNodeType == ASTNode::NodeType::Unknown)) {
					return false;
				}
				return true; }, offset);
	}

	ASTParser::ParseResult ASTParser::ExtractIrrational(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mIrrationalRegistry, expression, [&](const std::shared_ptr<Symbol>&) { return true; }, offset);
	}

	ASTParser::ParseResult ASTParser::ExtractParenthesis(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mParenthesisRegistry, expression, [&](const std::shared_ptr<Symbol>&) { return true; }, offset);
	}

	ASTParser::ParseResult ASTParser::ExtractUnknownVariable(const std::string& expression, const size_t& offset)
	{
		Symbol* symbol = new Symbol(expression.substr(offset, 1));
		return ParseResult(1, symbol);
	}

	void ASTParser::ReduceOperator(std::deque<ASTNode*>& operandStack, std::deque<ASTNode*>& operatorStack, OperatorNode* topOperator)
	{
		if (topOperator->mOperator->mType == OperatorType::Binary)
		{
			if (operandStack.size() < 2)
			{
				throw std::runtime_error("Invalid expression");
			}

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
			if (operandStack.empty())
			{
				throw std::runtime_error("Invalid expression");
			}

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
				offset += result.mExtractedLength;

				if (parenthesisNode->mParenthesis->mIsOpen)
				{
					operatorStack.push_back(parenthesisNode);
					lastNodeType = ASTNode::NodeType::Parenthesis;
					continue;
				}
				else // closing parenthesis
				{
					while (!operatorStack.empty())
					{
						ASTNode* top = operatorStack.back();
						if (top->mType == ASTNode::NodeType::Parenthesis)
						{
							ParenthesisNode* topParenthesisNode = dynamic_cast<ParenthesisNode*>(top);

							if ((!mMatchExactParenthesis &&
								!topParenthesisNode->mParenthesis->mIsOpen) ||
								(mMatchExactParenthesis &&
									topParenthesisNode->mParenthesis->IsOpposite(parenthesisNode->mParenthesis)))
							{
								operatorStack.pop_back();
								break;
							}
							else
							{
								// TODO: Handle mismatched parenthesis, report error
								return nullptr;
							}
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

			result = ExtractOperator(expression, lastNodeType, offset);
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
						if (operatorNode->mOperator->mPrecedence >= op->mPrecedence)
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

			// Unknown character fallback
			result = ExtractUnknownVariable(expression, offset);
			if (!result.HasError())
			{
				operandStack.push_back(new VariableNode(result.mSymbol));
				offset += result.mExtractedLength;
				lastNodeType = ASTNode::NodeType::Variable;
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