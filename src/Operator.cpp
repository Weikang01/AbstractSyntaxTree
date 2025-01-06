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
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 1, "-"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 2, "*"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::LeftToRight, 2, "/"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Unary, Associativity::RightToLeft, 3, "!"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::Binary, Associativity::RightToLeft, 4, "^"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionDual, Associativity::RightToLeft, 4, "root"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "sin"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "cos"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "tan"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "asin"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "acos"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "atan"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "sinh"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "cosh"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "tanh"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionDual, Associativity::RightToLeft, 5, "log"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "ln"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "sqrt"));
		defaultRegistry.RegisterSymbol(std::make_shared<Operator>(OperatorType::FunctionSingular, Associativity::RightToLeft, 5, "abs"));

		return defaultRegistry;
	}
}