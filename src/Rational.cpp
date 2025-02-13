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

	void Rational::operator++()
	{
		*this += 1;
	}

	void Rational::operator--()
	{
		*this -= 1;
	}

	int Rational::IntegerPower(int base, int exponent)
	{
		if (exponent < 0) {
			throw std::invalid_argument("Exponent must be non-negative in integer power.");
		}
		if (base == 0) {
			return 0;
		}
		int result = 1;
		while (exponent > 0) {
			if (exponent % 2 == 1) {
				result *= base;
			}
			base *= base;
			exponent /= 2;
		}
		return result;
	}

	bool Rational::IsPerfectRoot(int number, int rootDegree, int& root)
	{
		if (rootDegree <= 0) {
			return false;
		}
		if (rootDegree == 1) {
			root = number;
			return true;
		}
		if (number == 0) {
			root = 0;
			return true;
		}
		if (rootDegree % 2 == 0 && number < 0) {
			return false; // Even root of a negative number is not real
		}
		int abs_number = std::abs(number);
		int low = 1;
		int high = abs_number;
		while (low <= high) {
			int mid = low + (high - low) / 2;
			int power = 1;
			bool overflow = false;
			for (int i = 0; i < rootDegree; ++i) {
				if (mid != 0 && power > std::numeric_limits<int>::max() / mid) {
					overflow = true;
					break;
				}
				power *= mid;
				if (power > abs_number) {
					overflow = true;
					break;
				}
			}
			if (overflow) {
				high = mid - 1;
			}
			else if (power == abs_number) {
				root = mid;
				if (number < 0) {
					root = -root;
				}
				return true;
			}
			else if (power < abs_number) {
				low = mid + 1;
			}
			else {
				high = mid - 1;
			}
		}
		return false;
	}

	Rational Rational::Inverse() const
	{
		if (mNumerator == 0) {
			throw std::invalid_argument("Cannot invert zero.");
		}
		return Rational(mDenominator, mNumerator);
	}

	bool Rational::CanYieldRationalPowerResult(const Rational& exponent) const
	{
		if (exponent.mNumerator == 0) {
			return true; // Any number to the power of 0 is 1 (rational)
		}

		if (mNumerator == 0) {
			return exponent.mNumerator > 0; // 0^positive is 0 (rational), 0^negative is undefined
		}

		if (exponent.mDenominator == 1) {
			return true; // Integer exponent always yields a rational result
		}

		int root_num;
		bool numIsPerfect = IsPerfectRoot(mNumerator, exponent.mDenominator, root_num);
		int root_den;
		bool denIsPerfect = IsPerfectRoot(mDenominator, exponent.mDenominator, root_den);

		return numIsPerfect && denIsPerfect;
	}

	Rational Rational::Pow(const Rational& other) const
	{
		if (other.mNumerator == 0) {
			if (mNumerator == 0) {
				throw std::invalid_argument("Zero to the power of zero is undefined.");
			}
			else {
				return Rational(1);
			}
		}

		if (mNumerator == 0) {
			if (other.mNumerator > 0) {
				return Rational(0, 1);
			}
			else {
				throw std::invalid_argument("Zero cannot be raised to a negative power.");
			}
		}

		if (other.mDenominator == 1) {
			int exponent = other.mNumerator;
			bool negative_exponent = false;
			if (exponent < 0) {
				negative_exponent = true;
				exponent = -exponent;
			}
			int newNum = IntegerPower(mNumerator, exponent);
			int newDen = IntegerPower(mDenominator, exponent);
			if (negative_exponent) {
				return Rational(newDen, newNum).Inverse();
			}
			else {
				return Rational(newNum, newDen);
			}
		}

		int expNumerator = other.mNumerator;

		int rootNum;
		bool numIsPerfect = IsPerfectRoot(mNumerator, other.mDenominator, rootNum);
		int rootDen;
		bool denIsPerfect = IsPerfectRoot(mDenominator, other.mDenominator, rootDen);

		if (!numIsPerfect || !denIsPerfect) {
			throw std::invalid_argument("Exponent results in irrational number.");
		}

		if (expNumerator < 0) {
			std::swap(rootNum, rootDen);
			expNumerator = -expNumerator;
		}

		int newNum = IntegerPower(rootNum, expNumerator);
		int newDen = IntegerPower(rootDen, expNumerator);
		return Rational(newNum, newDen);
	}

	//bool Rational::CanYieldRationalLogResult(const Rational& base) const
	//{
	//	// TODO
	//}

	//Rational Rational::Log(const Rational& base) const
	//{
	//	// TODO
	//}

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

	Rational::operator int() const
	{
		return mNumerator / mDenominator;
	}

	Rational::operator float() const
	{
		return static_cast<float>(mNumerator) / mDenominator;
	}

	Rational::operator double() const
	{
		return static_cast<double>(mNumerator) / mDenominator;
	}
} // namespace LAR