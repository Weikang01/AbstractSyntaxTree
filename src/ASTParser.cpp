#include "ASTParser.h"

namespace AST
{
	ASTNode::NodeType GetNodeType(const ASTNode* node)
	{
		if (node)
		{
			return node->mType;
		}
		return ASTNode::NodeType::Unknown;
	}

	OperatorType GetOperatorNodeType(const ASTNode* node)
	{
		if (node && node->mType == ASTNode::NodeType::Operator)
		{
			const OperatorNode* opNode = dynamic_cast<const OperatorNode*>(node);
			return opNode->mOperator->mType;
		}
		return OperatorType::Invalid;
	}

	ASTParserSettings::ASTParserSettings()
	{
		if (mCustomSymbolRegistry == nullptr)
		{
			mCustomSymbolRegistry = new SymbolRegistry();
		}

		if (mImplicitOperatorInsertion)
		{
			if (mImplicitOperator == nullptr)
			{
				mImplicitOperator = std::dynamic_pointer_cast<Operator>(
					mOperatorRegistry->GetSymbol("*", [&](const std::shared_ptr<Symbol>&) { return true; })
				);
			}
			else if (mImplicitOperator->mType != OperatorType::Binary)
			{
				throw std::invalid_argument("Implicit operator must be a binary operator.");
			}
		}
	}

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

	ASTParser::ParseResult ASTParser::ExtractSymbol(const SymbolRegistry* symbolRegistry, const std::string& expression, const std::function<bool(const std::shared_ptr<Symbol>&)>& findFirstMatch, const size_t& offset)
	{
		if (expression.empty())
		{
			return ParseResult(0, ResultType::EmptyExpression);
		}

		std::shared_ptr<SymbolSearchNode> node = symbolRegistry->GetRoot();
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
		return ExtractSymbol(mSettings.mOperatorRegistry, expression, [&](const std::shared_ptr<Symbol>& symbol)
			{
				const Operator* op = dynamic_cast<const Operator*>(symbol.get());
				if (op->mType == OperatorType::Binary && (lastNodeType == ASTNode::NodeType::Operator || lastNodeType == ASTNode::NodeType::Unknown)) {
					return false;
				}
				return true; }, offset);
	}

	ASTParser::ParseResult ASTParser::ExtractIrrational(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mSettings.mIrrationalRegistry, expression, [&](const std::shared_ptr<Symbol>&) { return true; }, offset);
	}

	ASTParser::ParseResult ASTParser::ExtractParenthesis(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mSettings.mParenthesisRegistry, expression, [&](const std::shared_ptr<Symbol>&) { return true; }, offset);
	}

	ASTParser::ParseResult ASTParser::ExtractCustomSymbol(const std::string& expression, const size_t& offset)
	{
		return ExtractSymbol(mSettings.mCustomSymbolRegistry, expression, [&](const std::shared_ptr<Symbol>&) { return true; }, offset);
	}

	ASTParser::ParseResult ASTParser::ExtractUnknownVariable(const std::string& expression, const size_t& offset)
	{
		if (expression.size() > offset)
		{
			Symbol* symbol = new Symbol(expression.substr(offset, 1));
			return ParseResult(1, symbol);
		}
		else
		{
			return ParseResult(0, ResultType::InvalidExpression);
		}
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

	void ASTParser::HandleOperatorExtraction(std::deque<ASTNode*>& operandStack, std::deque<ASTNode*>& operatorStack, const Symbol* operatorSymbol)
	{
		const Operator* op = dynamic_cast<const Operator*>(operatorSymbol);
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
	}

	void ASTParser::ImplicitOperatorInsertion(std::deque<ASTNode*>& operandStack, std::deque<ASTNode*>& operatorStack, const ASTNode* lastNode)
	{
		if (!mSettings.mImplicitOperatorInsertion || GetNodeType(lastNode) == ASTNode::NodeType::Operator || GetNodeType(lastNode) == ASTNode::NodeType::Unknown)
		{
			return;
		}

		if (operatorStack.size() != 0)
		{
			ASTNode* top = operatorStack.back();
			if (top->mType == ASTNode::NodeType::Parenthesis)
			{
				ParenthesisNode* topParenthesisNode = dynamic_cast<ParenthesisNode*>(top);
				if (topParenthesisNode->mParenthesis->mIsOpen)
				{
					return;
				}
			}
		}

		HandleOperatorExtraction(operandStack, operatorStack, mSettings.mImplicitOperator.get());
	}

	bool ShouldParseRational(const ASTNode* lastNode)
	{
		const ASTNode::NodeType lastNodeType = GetNodeType(lastNode);
		if (lastNodeType == ASTNode::NodeType::Parenthesis)
		{
			const ParenthesisNode* lastParenthesisNode = dynamic_cast<const ParenthesisNode*>(lastNode);
			return lastParenthesisNode->mParenthesis->mIsOpen;
		}

		return lastNodeType != ASTNode::NodeType::Rational;
	}

	ASTNode* ASTParser::Parse(const std::string& expression)
	{
		std::deque<ASTNode*> operandStack;
		std::deque<ASTNode*> operatorStack;
		ASTNode* lastNode = nullptr;

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
				ImplicitOperatorInsertion(operandStack, operatorStack, lastNode);

				ParenthesisNode* parenthesisNode = new ParenthesisNode(result.mSymbol);
				offset += result.mExtractedLength;
				lastNode = parenthesisNode;

				if (parenthesisNode->mParenthesis->mIsOpen)
				{
					operatorStack.push_back(parenthesisNode);
				}
				else // closing parenthesis
				{
					while (!operatorStack.empty())
					{
						ASTNode* top = operatorStack.back();
						if (top->mType == ASTNode::NodeType::Parenthesis)
						{
							ParenthesisNode* topParenthesisNode = dynamic_cast<ParenthesisNode*>(top);

							if ((!mSettings.mMatchExactParenthesis &&
								!topParenthesisNode->mParenthesis->mIsOpen) ||
								(mSettings.mMatchExactParenthesis &&
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
				continue;
			}

			if (ShouldParseRational(lastNode))
			{
				result = ExtractRational(expression, offset);
				if (!result.HasError())
				{
					ImplicitOperatorInsertion(operandStack, operatorStack, lastNode);

					operandStack.push_back(new RationalNode(ParseRational(expression, offset, result.mExtractedLength)));
					offset += result.mExtractedLength;
					lastNode = operandStack.back();
					continue;
				}
			}

			result = ExtractIrrational(expression, offset);
			if (!result.HasError())
			{
				ImplicitOperatorInsertion(operandStack, operatorStack, lastNode);

				operandStack.push_back(new IrrationalNode(result.mSymbol));
				offset += result.mExtractedLength;
				lastNode = operandStack.back();
				continue;
			}

			result = ExtractOperator(expression, GetNodeType(lastNode), offset);
			if (!result.HasError())
			{
				const Operator* op = dynamic_cast<const Operator*>(result.mSymbol);
				if (op->mType == OperatorType::FunctionDual || op->mType == OperatorType::FunctionMultiple || op->mType == OperatorType::FunctionSingular)
				{
					ImplicitOperatorInsertion(operandStack, operatorStack, lastNode);
				}

				HandleOperatorExtraction(operandStack, operatorStack, result.mSymbol);
				offset += result.mExtractedLength;
				lastNode = operatorStack.back();

				continue;
			}

			result = ExtractCustomSymbol(expression, offset);
			if (!result.HasError())
			{
				ImplicitOperatorInsertion(operandStack, operatorStack, lastNode);

				operandStack.push_back(new VariableNode(result.mSymbol));
				offset += result.mExtractedLength;
				lastNode = operandStack.back();
				continue;
			}

			// Unknown character fallback
			result = ExtractUnknownVariable(expression, offset);
			if (!result.HasError())
			{
				ImplicitOperatorInsertion(operandStack, operatorStack, lastNode);

				operandStack.push_back(new VariableNode(result.mSymbol));
				offset += result.mExtractedLength;
				lastNode = operandStack.back();
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

	void ASTParser::RegisterCustomSymbol(const std::string& symbol)
	{
		mSettings.mCustomSymbolRegistry->RegisterSymbol(std::make_shared<Symbol>(symbol));
	}

	void ASTParser::UnregisterCustomSymbol(const std::string& symbol)
	{
		mSettings.mCustomSymbolRegistry->UnregisterSymbol(symbol);
	}
}