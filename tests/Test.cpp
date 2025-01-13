#include "Rational.h"
#include "ASTParser.h"
#include "ASTNodeTreeViewer.h"
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
	ASTParser mParser;

	ParserTest()
	{
		mParser = ASTParser();
	}

	ASTParser::ParseResult TestExtractRational(const std::string& expression)
	{
		return mParser.ExtractRational(expression);
	}

	static OperatorNode* CreateOperatorNode(const std::string& symbol, std::vector<ASTNode*> operands)
	{
		Symbol* symbolPtr = new Symbol(symbol);
		OperatorNode* node = new OperatorNode(symbolPtr);
		node->mOperator = new Operator(OperatorType::FunctionMultiple, Associativity::LeftToRight, 0, symbol);
		node->mOperands = operands;
		return node;
	}
};

TEST_CASE("ParserTest", "[ParserTest]")
{
	ParserTest parserTest;
	SECTION("ExtractRational")
	{
		ASTParser::ParseResult result = parserTest.TestExtractRational("123");
		REQUIRE(result.mExtractedLength == 3);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.TestExtractRational("123.456");
		REQUIRE(result.mExtractedLength == 7);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.TestExtractRational("123.");
		REQUIRE(result.mExtractedLength == 4);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.TestExtractRational(".456");
		REQUIRE(result.mExtractedLength == 4);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.TestExtractRational("123.456.789");
		REQUIRE(result.mErrorPos == 7);
		REQUIRE(result.mErrorType == ASTParser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("hello");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == ASTParser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == ASTParser::ResultType::EmptyExpression);
		result = parserTest.TestExtractRational("+");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == ASTParser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("-");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == ASTParser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational(".");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == ASTParser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("-.");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == ASTParser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("+.");
		REQUIRE(result.mErrorPos == 0);
		REQUIRE(result.mErrorType == ASTParser::ResultType::InvalidNumberFormat);
		result = parserTest.TestExtractRational("-123.");
		REQUIRE(result.mExtractedLength == 5);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.TestExtractRational("+123.");
		REQUIRE(result.mExtractedLength == 5);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
	}

	SECTION("ExtractOperator")
	{
		ASTParser::ParseResult result = parserTest.mParser.ExtractOperator("123");
		REQUIRE(result.mExtractedLength == 0);
		REQUIRE(result.mErrorType == ASTParser::ResultType::InvalidOperator);
		result = parserTest.mParser.ExtractOperator("+");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("-");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("*");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("/");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("++");
		REQUIRE(result.mExtractedLength == 1);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("sinhabc");
		REQUIRE(result.mExtractedLength == 4);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("cosabc");
		REQUIRE(result.mExtractedLength == 3);
		REQUIRE(result.mErrorType == ASTParser::ResultType::NoError);
		result = parserTest.mParser.ExtractOperator("taa");
		REQUIRE(result.mErrorType == ASTParser::ResultType::InvalidOperator);
	}

	//SECTION("Parse")
	//{
	//	ASTNode* node = parserTest.mParser.Parse("-1+1");
	//	ASTNodeTreeViewer::PrintTree(node);
	//}

	SECTION("ASTNodeTreeViewer")
	{
		OperatorNode* testOp1 = ParserTest::CreateOperatorNode("111111111111111111111111111111111111111111111111111111111111111", {});
		OperatorNode* testOp2 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp3 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp4 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp5 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp6 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp7 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp8 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp9 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp10 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp11 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp12 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp13 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp14 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp15 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp16 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp17 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp18 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp19 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp20 = ParserTest::CreateOperatorNode("1", {});
		OperatorNode* testOp21 = ParserTest::CreateOperatorNode("1", {});

		testOp1->mOperands.push_back(testOp2);
		testOp1->mOperands.push_back(testOp3);
		testOp1->mOperands.push_back(testOp4);
		testOp1->mOperands.push_back(testOp5);

		testOp2->mOperands.push_back(testOp6);
		testOp2->mOperands.push_back(testOp7);

		testOp3->mOperands.push_back(testOp8);

		testOp4->mOperands.push_back(testOp9);
		testOp4->mOperands.push_back(testOp10);
		testOp4->mOperands.push_back(testOp11);
		testOp4->mOperands.push_back(testOp12);
		testOp4->mOperands.push_back(testOp21);

		testOp5->mOperands.push_back(testOp13);
		testOp5->mOperands.push_back(testOp14);
		testOp5->mOperands.push_back(testOp15);

		testOp6->mOperands.push_back(testOp16);
		testOp6->mOperands.push_back(testOp17);

		testOp7->mOperands.push_back(testOp18);
		testOp7->mOperands.push_back(testOp19);

		testOp8->mOperands.push_back(testOp20);


		ASTNodeTreeViewer::PrintTree(testOp1);
	}
}

}
#endif // UNIT_TEST