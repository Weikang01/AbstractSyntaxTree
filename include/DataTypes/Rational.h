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

		static int IntegerPower(int base, int exponent);
		static bool IsPerfectRoot(int number, int rootDegree, int& root);
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
		void operator++();
		void operator--();

		Rational Inverse() const;

		bool CanYieldRationalPowerResult(const Rational& exponent) const;
		Rational Pow(const Rational& other) const;

		bool CanYieldRationalLogResult(const Rational& base) const;
		Rational Log(const Rational& base) const;

		static Rational FromString(const std::string& str);
		std::string ToString() const;

		explicit operator int() const;
		explicit operator float() const;
		explicit operator double() const;
	};

	Rational operator+(const int value, const Rational& rational);
	Rational operator-(const int value, const Rational& rational);
	Rational operator*(const int value, const Rational& rational);
	Rational operator/(const int value, const Rational& rational);

	std::ostream& operator<<(std::ostream& os, const Rational& rational);
}