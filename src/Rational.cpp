#include "Rational.h"
#include <numeric> // For std::gcd
#include <sstream>

namespace AST
{
	void Rational::Simplify()
	{
		int gcd = std::gcd(mNumerator, mDenominator);
		mNumerator /= gcd;
		mDenominator /= gcd;

		if (mDenominator < 0)
		{
			mNumerator = -mNumerator;
			mDenominator = -mDenominator;
		}
	}
	void Rational::SimplifyForOperation(const Rational& r1, const Rational& r2, int& num1, int& den1, int& num2, int& den2)
	{
		int gcd1 = std::gcd(r1.mNumerator, r2.mDenominator);
		int gcd2 = std::gcd(r2.mNumerator, r1.mDenominator);
		num1 = r1.mNumerator / gcd1;
		den1 = r1.mDenominator / gcd2;
		num2 = r2.mNumerator / gcd2;
		den2 = r2.mDenominator / gcd1;
	}
	;

	Rational::Rational(int numerator, int denominator)
	{
		if (denominator == 0)
			throw std::invalid_argument("Denominator cannot be zero.");
		mNumerator = numerator;
		mDenominator = denominator;

		Simplify();
	}

	Rational Rational::operator+(const Rational& other) const
	{
		int num1, den1, num2, den2;
		SimplifyForOperation(*this, other, num1, den1, num2, den2);

		int newNumerator = num1 * den2 + num2 * den1;
		int newDenominator = den1 * den2;

		return Rational(newNumerator, newDenominator);
	}

	Rational Rational::operator-(const Rational& other) const
	{
		int num1, den1, num2, den2;
		SimplifyForOperation(*this, other, num1, den1, num2, den2);
		
		int newNumerator = num1 * den2 - num2 * den1;
		int newDenominator = den1 * den2;

		return Rational(newNumerator, newDenominator);
	}

	Rational Rational::operator*(const Rational& other) const
	{
		int num1, den1, num2, den2;
		SimplifyForOperation(*this, other, num1, den1, num2, den2);

		return Rational(num1 * num2, den1 * den2);
	}

	Rational Rational::operator/(const Rational& other) const
	{
		if (other.mNumerator == 0)
		{
			throw std::invalid_argument("Division by zero in Rational");
		}

		int num1, den1, num2, den2;
		SimplifyForOperation(*this, other, num1, den1, num2, den2);

		return Rational(num1 * den2, den1 * num2);
	}

	Rational& Rational::operator+=(const Rational& other)
	{
		*this = *this + other;
		return *this;
	}

	Rational& Rational::operator-=(const Rational& other)
	{
		*this = *this - other;
		return *this;
	}

	Rational& Rational::operator*=(const Rational& other)
	{
		*this = *this * other;
		return *this;
	}

	Rational& Rational::operator/=(const Rational& other)
	{
		*this = *this / other;
		return *this;
	}

	Rational Rational::operator-() const
	{
		return Rational(-mNumerator, mDenominator);
	}

	bool Rational::operator==(const Rational& other) const
	{
		return mNumerator == other.mNumerator && mDenominator == other.mDenominator;
	}

	bool Rational::operator!=(const Rational& other) const
	{
		return !(*this == other);
	}

	bool Rational::operator<(const Rational& other) const
	{
		return mNumerator * other.mDenominator < other.mNumerator * mDenominator;
	}

	Rational Rational::FromString(const std::string& str)
	{
		std::istringstream iss(str);
		int num = 0, denom = 1;
		char separator = '\0';

		if (str.find('/') != std::string::npos) { // Fraction
			if (!(iss >> num >> separator >> denom) || separator != '/' || denom == 0) {
				throw std::invalid_argument("Invalid fraction format.");
			}
		}
		else if (str.find('.') != std::string::npos) { // Decimal
			size_t decimalPos = str.find('.');
			std::string integerPart = str.substr(0, decimalPos);
			std::string fractionalPart = str.substr(decimalPos + 1);

			// Remove trailing zeros
			fractionalPart.erase(fractionalPart.find_last_not_of('0') + 1);

			size_t fractionalLength = fractionalPart.length();
			num = std::stoi(integerPart + fractionalPart);
			denom = static_cast<int>(std::pow(10, fractionalLength));
		}
		else { // Integer
			num = std::stoi(str);
		}

		return Rational(num, denom);
	}

	Rational operator+(const int value, const Rational& rational)
	{
		return Rational(value) + rational;
	}

	Rational operator-(const int value, const Rational& rational)
	{
		return Rational(value) - rational;
	}

	Rational operator*(const int value, const Rational& rational)
	{
		return Rational(value) * rational;
	}

	Rational operator/(const int value, const Rational& rational)
	{
		return Rational(value) / rational;
	}

	std::ostream& operator<<(std::ostream& os, const Rational& rational)
	{
		os << rational.GetNumerator();
		if (rational.GetDenominator() != 1)
		{
			os << "/" << rational.GetDenominator();
		}
		return os;
	}

} // namespace LAR