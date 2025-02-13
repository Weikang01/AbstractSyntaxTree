#pragma once

namespace AST
{
	enum class OperationId : uint8_t
	{
		Invalid = 0,
		Addition,          // Binary +
		UnaryPlus,         // Unary +
		Subtraction,       // Binary -
		UnaryMinus,        // Unary -
		Multiplication,    // Binary *
		Division,          // Binary /
		Factorial,         // Unary !
		Exponentiation,    // Binary ^
		Root,              // Dual-argument function "root"
		Sine,              // Singular function "sin"
		Cosine,            // Singular function "cos"
		Tangent,           // Singular function "tan"
		ArcSine,           // Singular function "asin"
		ArcCosine,         // Singular function "acos"
		ArcTangent,        // Singular function "atan"
		HyperbolicSine,    // Singular function "sinh"
		HyperbolicCosine,  // Singular function "cosh"
		HyperbolicTangent, // Singular function "tanh"
		Logarithm,         // Dual-argument function "log"
		NaturalLogarithm,  // Singular function "ln"
		SquareRoot,        // Singular function "sqrt"
		AbsoluteValue      // Singular function "abs"
	};
} // namespace AST