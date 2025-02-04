#include "Operator.h"
#include <iostream>
#include <vector>
#include <map>

namespace AST
{
	const OperatorRegistry& OperatorRegistry::GetDefaultRegistry()
	{
		static OperatorRegistry defaultRegistry;
		// Register operators
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 1, "+", OperationId::Addition));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Unary, Associativity::LeftToRight, 4, "+", OperationId::UnaryPlus));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 1, "-", OperationId::Subtraction));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Unary, Associativity::LeftToRight, 4, "-", OperationId::UnaryMinus));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 2, "*", OperationId::Multiplication));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 2, "/", OperationId::Division));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Unary, Associativity::RightToLeft, 5, "!", OperationId::Factorial));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::RightToLeft, 3, "^", OperationId::Exponentiation));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionDual, Associativity::RightToLeft, 6, "root", OperationId::Root));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "sin", OperationId::Sine));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "cos", OperationId::Cosine));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "tan", OperationId::Tangent));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "asin", OperationId::ArcSine));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "acos", OperationId::ArcCosine));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "atan", OperationId::ArcTangent));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "sinh", OperationId::HyperbolicSine));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "cosh", OperationId::HyperbolicCosine));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "tanh", OperationId::HyperbolicTangent));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionDual, Associativity::RightToLeft, 6, "log", OperationId::Logarithm));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "ln", OperationId::NaturalLogarithm));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "sqrt", OperationId::SquareRoot));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "abs", OperationId::AbsoluteValue));

		return defaultRegistry;
	}
}