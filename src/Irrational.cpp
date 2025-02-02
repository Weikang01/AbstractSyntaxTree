#include "Irrational.h"

namespace AST
{
	const IrrationalRegistry& IrrationalRegistry::GetDefaultRegistry()
	{
		static IrrationalRegistry defaultRegistry;
		// Register symbols
		defaultRegistry.RegisterSymbol(std::make_shared<Irrational>("pi", 3.14159265358979323846));
		defaultRegistry.RegisterSymbol(std::make_shared<Irrational>("e", 2.71828182845904523536));
		defaultRegistry.RegisterSymbol(std::make_shared<Irrational>("phi", 1.61803398874989484820));
		return defaultRegistry;
	}
}