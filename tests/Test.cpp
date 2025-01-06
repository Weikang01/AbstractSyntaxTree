#include "Rational.h"
#include "AST.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#ifdef UNIT_TEST
namespace AST
{
TEST_CASE("RationalTest", "[RationalTest]")
{
	Rational r1(12);  // 12/1
	Rational r2(4, 6); // 2/3
	Rational r3(3, 4); // 3/4
	Rational r4(1, 2); // 1/2

	REQUIRE(r1.GetNumerator() == 12);
	REQUIRE(r1.GetDenominator() == 1);

	Rational r5 = Rational::FromString("1/2");
	REQUIRE(r5.GetNumerator() == 1);
	REQUIRE(r5.GetDenominator() == 2);

	Rational r6 = Rational::FromString("0.5");
	REQUIRE(r6.GetNumerator() == 1);
	REQUIRE(r6.GetDenominator() == 2);
}

TEST_CASE("RationalOperatorsTest", "[RationalOperatorsTest]")
{
	Rational r1(12);  // 12/1
	Rational r2(4, 6); // 2/3
	Rational r3(3, 4); // 3/4
	Rational r4(1, 2); // 1/2
	SECTION("Addition")
	{
		Rational r5 = r1 + r2;
		REQUIRE(r5.GetNumerator() == 38);
		REQUIRE(r5.GetDenominator() == 3);
	}
	SECTION("Subtraction")
	{
		Rational r5 = r1 - r2;
		REQUIRE(r5.GetNumerator() == 34);
		REQUIRE(r5.GetDenominator() == 3);
	}
	SECTION("Multiplication")
	{
		Rational r5 = r1 * r2;
		REQUIRE(r5.GetNumerator() == 8);
		REQUIRE(r5.GetDenominator() == 1);
	}
	SECTION("Division")
	{
		Rational r5 = r1 / r2;
		REQUIRE(r5.GetNumerator() == 18);
		REQUIRE(r5.GetDenominator() == 1);
	}
	SECTION("AdditionAssignment")
	{
		Rational r5 = r1;
		r5 += r2;
		REQUIRE(r5.GetNumerator() == 38);
		REQUIRE(r5.GetDenominator() == 3);
	}
	SECTION("SubtractionAssignment")
	{
		Rational r5 = r1;
		r5 -= r2;
		REQUIRE(r5.GetNumerator() == 34);
		REQUIRE(r5.GetDenominator() == 3);
	}
	SECTION("MultiplicationAssignment")
	{
		Rational r5 = r1;
		r5 *= r2;
		REQUIRE(r5.GetNumerator() == 8);
		REQUIRE(r5.GetDenominator() == 1);
	}
	SECTION("DivisionAssignment")
	{
		Rational r5 = r1;
		r5 /= r2;
		REQUIRE(r5.GetNumerator() == 18);
		REQUIRE(r5.GetDenominator() == 1);
	}
}

TEST_CASE("RationalComparisonTest", "[RationalComparisonTest]")
{
	Rational r1(12);  // 12/1
	Rational r2(4, 6); // 2/3
	Rational r3(3, 4); // 3/4
	Rational r4(1, 2); // 1/2
	SECTION("Equality")
	{
		REQUIRE(r1 == Rational(12));
		REQUIRE(r2 == Rational(2, 3));
		REQUIRE(r3 == Rational(3, 4));
		REQUIRE(r4 == Rational(1, 2));
	}
	SECTION("Inequality")
	{
		REQUIRE(r1 != Rational(1));
		REQUIRE(r2 != Rational(1, 3));
		REQUIRE(r3 != Rational(1, 4));
		REQUIRE(r4 != Rational(1, 3));
	}
	SECTION("LessThan")
	{
		REQUIRE(r1 < Rational(13));
		REQUIRE(r2 < Rational(3, 2));
		REQUIRE(r3 < Rational(4, 3));
		REQUIRE(r4 < Rational(2, 3));
	}
}

TEST_CASE("RationalUnaryOperatorsTest", "[RationalUnaryOperatorsTest]")
{
	Rational r1(12);  // 12/1
	Rational r2(4, 6); // 2/3
	Rational r3(3, 4); // 3/4
	Rational r4(1, 2); // 1/2
	SECTION("Negation")
	{
		Rational r5 = -r1;
		REQUIRE(r5.GetNumerator() == -12);
		REQUIRE(r5.GetDenominator() == 1);
	}
}

class ParserTest
{
public:
	Parser mParser;

	ParserTest()
	{
		mParser = Parser();
	}

	Parser::ParseResult TestExtractRational(const std::string& expression)
	{
		return mParser.ExtractRational(expression);
	}
};

TEST_CASE("ParserTest", "[ParserTest]")
{
	ParserTest parserTest;
	SECTION("ExtractRational")
	{
		Parser::ParseResult result = parserTest.TestExtractRational("123");
		REQUIRE(result.mExtractedLength == 3);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.TestExtractRational("123.456");
		REQUIRE(result.mExtractedLength == 7);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.TestExtractRational("123.");
		REQUIRE(result.mExtractedLength == 4);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.TestExtractRational(".456");
		REQUIRE(result.mExtractedLength == 4);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.TestExtractRational("123.456.789");
		REQUIRE(result.mErrorPos == 7);
		REQUIRE(result.mErrorType == Parser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("hello");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == Parser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == Parser::ResultType::EmptyExpression);
		result = parserTest.TestExtractRational("+");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == Parser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("-");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == Parser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational(".");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == Parser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("-.");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == Parser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("+.");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == Parser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("-123.");
		REQUIRE(result.mExtractedLength == 5);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.TestExtractRational("+123.");
		REQUIRE(result.mExtractedLength == 5);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
	}

	SECTION("ExtractOperator")
	{
		Parser::ParseResult result = parserTest.mParser.ExtractOperator("123");
		REQUIRE(result.mExtractedLength == 0);
		REQUIRE(result.mErrorType == Parser::ResultType::InvalidOperator);
		result = parserTest.mParser.ExtractOperator("+");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("-");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("*");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("/");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("++");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("sinhabc");
		REQUIRE(result.mExtractedLength == 4);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("cosabc");
		REQUIRE(result.mExtractedLength == 3);
		REQUIRE(result.mErrorType == Parser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("taa");
		REQUIRE(result.mErrorType == Parser::ResultType::InvalidOperator);
	}
}

}
#endif // UNIT_TEST