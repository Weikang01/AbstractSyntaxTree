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
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 1, "+"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Unary, Associativity::LeftToRight, 4, "+"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 1, "-"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Unary, Associativity::LeftToRight, 4, "-"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 2, "*"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 2, "/"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Unary, Associativity::RightToLeft, 5, "!"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::RightToLeft, 3, "^"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionDual, Associativity::RightToLeft, 6, "root"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "sin"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "cos"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "tan"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "asin"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "acos"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "atan"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "sinh"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "cosh"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "tanh"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionDual, Associativity::RightToLeft, 6, "log"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "ln"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "sqrt"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 6, "abs"));

		return defaultRegistry;
	}
}