#include "Rational.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <thread>
#include <chrono>

TEST_CASE("RationalTest", "[RationalTest]")
{
	AST::Rational r1(12);  // 12/1
	AST::Rational r2(4, 6); // 2/3
	AST::Rational r3(3, 4); // 3/4
	AST::Rational r4(1, 2); // 1/2

	REQUIRE(r1.GetNumerator() == 12);
	REQUIRE(r1.GetDenominator() == 1);

	AST::Rational r5 = AST::Rational::FromString("1/2");
	REQUIRE(r5.GetNumerator() == 1);
	REQUIRE(r5.GetDenominator() == 2);

	AST::Rational r6 = AST::Rational::FromString("0.5");
	REQUIRE(r6.GetNumerator() == 1);
	REQUIRE(r6.GetDenominator() == 2);
}