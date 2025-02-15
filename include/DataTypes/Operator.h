#pragma once
#include "DataTypes/Symbol.h"
#include "DataTypes/Operation.h"

namespace AST
{
	enum class OperatorType : uint8_t
	{
		Invalid,
		Unary, // Unary operators have only one operand
		Binary, // Binary operators have two operands
		FunctionSingular, // Functions with one argument, e.g. sin, cos, tan
		FunctionDual, // Functions with two arguments, e.g. root, log
		FunctionMultiple, // Functions with multiple arguments, e.g. sum, product
	};

	enum class Associativity : uint8_t
	{
		LeftToRight,
		RightToLeft,
	};

	struct Operator : public Symbol
	{
		OperatorType mType;
		Associativity mAssociativity;
		uint16_t mPrecedence;
		OperationId mOperationId = OperationId::Invalid;

		Operator(OperatorType type, Associativity associativity, uint16_t precedence, const std::string& symbol, const OperationId& operationId)
			: Symbol(symbol), mType(type), mAssociativity(associativity), mPrecedence(precedence), mOperationId(operationId)
		{
		};

		virtual bool operator==(const Symbol& other) const override;

		virtual ~Operator() = default;
	};

	class OperatorRegistry : public SymbolRegistry
	{
	public:
		static const OperatorRegistry& GetDefaultRegistry();
	};
}