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

TEST_CASE("RationalOperatorsTest", "[RationalOperatorsTest]")
{
	AST::Rational r1(12);  // 12/1
	AST::Rational r2(4, 6); // 2/3
	AST::Rational r3(3, 4); // 3/4
	AST::Rational r4(1, 2); // 1/2
	SECTION("Addition")
	{
		AST::Rational r5 = r1 + r2;
		REQUIRE(r5.GetNumerator() == 38);
		REQUIRE(r5.GetDenominator() == 3);
	}
	SECTION("Subtraction")
	{
		AST::Rational r5 = r1 - r2;
		REQUIRE(r5.GetNumerator() == 34);
		REQUIRE(r5.GetDenominator() == 3);
	}
	SECTION("Multiplication")
	{
		AST::Rational r5 = r1 * r2;
		REQUIRE(r5.GetNumerator() == 8);
		REQUIRE(r5.GetDenominator() == 1);
	}
	SECTION("Division")
	{
		AST::Rational r5 = r1 / r2;
		REQUIRE(r5.GetNumerator() == 18);
		REQUIRE(r5.GetDenominator() == 1);
	}
	SECTION("AdditionAssignment")
	{
		AST::Rational r5 = r1;
		r5 += r2;
		REQUIRE(r5.GetNumerator() == 38);
		REQUIRE(r5.GetDenominator() == 3);
	}
	SECTION("SubtractionAssignment")
	{
		AST::Rational r5 = r1;
		r5 -= r2;
		REQUIRE(r5.GetNumerator() == 34);
		REQUIRE(r5.GetDenominator() == 3);
	}
	SECTION("MultiplicationAssignment")
	{
		AST::Rational r5 = r1;
		r5 *= r2;
		REQUIRE(r5.GetNumerator() == 8);
		REQUIRE(r5.GetDenominator() == 1);
	}
	SECTION("DivisionAssignment")
	{
		AST::Rational r5 = r1;
		r5 /= r2;
		REQUIRE(r5.GetNumerator() == 18);
		REQUIRE(r5.GetDenominator() == 1);
	}
}