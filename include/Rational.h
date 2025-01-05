#pragma once
#include <iostream>

namespace AST
{
	class Rational
	{
	private:
		int mNumerator;
		int mDenominator;

		void Simplify();

		static void SimplifyForOperation(const Rational& r1, const Rational& r2,
			int& num1, int& den1, int& num2, int& den2);

	public:
		Rational(int numerator = 0, int denominator = 1);

		int GetNumerator() const { return mNumerator; }
		int GetDenominator() const { return mDenominator; }

		Rational operator+(const Rational& other) const;
		Rational operator-(const Rational& other) const;
		Rational operator*(const Rational& other) const;
		Rational operator/(const Rational& other) const;

		Rational& operator+=(const Rational& other);
		Rational& operator-=(const Rational& other);
		Rational& operator*=(const Rational& other);
		Rational& operator/=(const Rational& other);

		Rational operator-() const;
		bool operator==(const Rational& other) const;
		bool operator!=(const Rational& other) const;
		bool operator<(const Rational& other) const;

		static Rational FromString(const std::string& str);
	};

	Rational operator+(const int value, const Rational& rational);
	Rational operator-(const int value, const Rational& rational);
	Rational operator*(const int value, const Rational& rational);
	Rational operator/(const int value, const Rational& rational);

	std::ostream& operator<<(std::ostream& os, const Rational& rational);
}