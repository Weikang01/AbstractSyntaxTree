#include "DataTypes/Operator.h"
#include <iostream>
#include <vector>
#include <map>

namespace AST
{
	bool Operator::operator==(const Symbol& other) const
	{
		if (const auto* rhs = dynamic_cast<const Operator*>(&other))
		{
			return mSymbol == rhs->mSymbol && mType == rhs->mType && mAssociativity == rhs->mAssociativity && mPrecedence == rhs->mPrecedence && mOperationId == rhs->mOperationId;
		}
		return false;
	}

	void OperatorRegistry::RegisterOperator(const OperatorType& type, const Associativity& associativity, const uint16_t& precedence, const std::string& symbol, const OperationId& operationId)
	{
		std::shared_ptr<Operator> op = std::make_shared<Operator>(type, associativity, precedence, symbol, operationId);
		mOperatorMap[operationId] = op;
		RegisterSymbol(op);
	}

	OperatorRegistry* OperatorRegistry::GetDefaultRegistry()
	{
		static OperatorRegistry* defaultRegistry = new OperatorRegistry();
		static bool initialized = false;

		if (initialized)
		{
			return defaultRegistry;
		}

		initialized = true;
		// Register operators
		defaultRegistry->RegisterOperator(OperatorType::Binary, Associativity::LeftToRight, 1, "+", OperationId::Addition);
		defaultRegistry->RegisterOperator(OperatorType::Unary, Associativity::LeftToRight, 4, "+", OperationId::UnaryPlus);
		defaultRegistry->RegisterOperator(OperatorType::Binary, Associativity::LeftToRight, 1, "-", OperationId::Subtraction);
		defaultRegistry->RegisterOperator(OperatorType::Unary, Associativity::LeftToRight, 4, "-", OperationId::UnaryMinus);
		defaultRegistry->RegisterOperator(OperatorType::Binary, Associativity::LeftToRight, 2, "*", OperationId::Multiplication);
		defaultRegistry->RegisterOperator(OperatorType::Binary, Associativity::LeftToRight, 2, "/", OperationId::Division);
		defaultRegistry->RegisterOperator(OperatorType::Unary, Associativity::RightToLeft, 5, "!", OperationId::Factorial);
		defaultRegistry->RegisterOperator(OperatorType::Binary, Associativity::RightToLeft, 3, "^", OperationId::Exponentiation);
		defaultRegistry->RegisterOperator(OperatorType::FunctionDual, Associativity::RightToLeft, 6, "root", OperationId::Root);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "sin", OperationId::Sine);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "cos", OperationId::Cosine);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "tan", OperationId::Tangent);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "asin", OperationId::ArcSine);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "acos", OperationId::ArcCosine);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "atan", OperationId::ArcTangent);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "sinh", OperationId::HyperbolicSine);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "cosh", OperationId::HyperbolicCosine);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "tanh", OperationId::HyperbolicTangent);
		defaultRegistry->RegisterOperator(OperatorType::FunctionDual, Associativity::RightToLeft, 6, "log", OperationId::Logarithm);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "ln", OperationId::NaturalLogarithm);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "sqrt", OperationId::SquareRoot);
		defaultRegistry->RegisterOperator(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "abs", OperationId::AbsoluteValue);

		return defaultRegistry;
	}

	std::shared_ptr<Operator> OperatorRegistry::GetOperator(const OperationId& operationId) const
	{
		auto it = mOperatorMap.find(operationId);
		if (it == mOperatorMap.end())
		{
			return nullptr;
		}
		return it->second;
	}
}