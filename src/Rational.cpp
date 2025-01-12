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
		int gcdDen = std::gcd(mDenominator, other.mDenominator);
		int newDen = mDenominator * other.mDenominator / gcdDen;
		int gcdNum = std::gcd(newDen, std::gcd(mNumerator, other.mNumerator));
		int newNum = mNumerator * (newDen / mDenominator) + other.mNumerator * (newDen / other.mDenominator);
		newDen /= gcdNum;

		return Rational(newNum, newDen);
	}

	Rational Rational::operator-(const Rational& other) const
	{
		int gcdDen = std::gcd(mDenominator, other.mDenominator);
		int newDen = mDenominator * other.mDenominator / gcdDen;
		int gcdNum = std::gcd(newDen, std::gcd(mNumerator, other.mNumerator));
		int newNum = mNumerator * (newDen / mDenominator) - other.mNumerator * (newDen / other.mDenominator);

		return Rational(newNum, newDen);
	}

	Rational Rational::operator*(const Rational& other) const
	{
		int gcd1 = std::gcd(mNumerator, other.mDenominator);
		int gcd2 = std::gcd(other.mNumerator, mDenominator);
		int num1 = mNumerator / gcd1;
		int den1 = mDenominator / gcd2;
		int num2 = other.mNumerator / gcd2;
		int den2 = other.mDenominator / gcd1;

		return Rational(num1 * num2, den1 * den2);
	}

	Rational Rational::operator/(const Rational& other) const
	{
		if (other.mNumerator == 0)
		{
			throw std::invalid_argument("Division by zero in Rational");
		}

		int gcd1 = std::gcd(mNumerator, other.mNumerator);
		int gcd2 = std::gcd(mDenominator, other.mDenominator);
		int num1 = mNumerator / gcd1;
		int den1 = mDenominator / gcd2;
		int num2 = other.mDenominator / gcd2;
		int den2 = other.mNumerator / gcd1;

		return Rational(num1 * num2, den1 * den2);
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

	std::string Rational::ToString() const
	{
		std::ostringstream oss;
		oss << mNumerator;
		if (mDenominator != 1)
		{
			oss << "/" << mDenominator;
		}
		return oss.str();
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