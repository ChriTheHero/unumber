//
// Created by Kirill Zhukov on 29.03.2025.
//

#include "unumber/rational/BigRational.h"


namespace usub::mp
{
    BigRational::BigRational() : value{0}
    {
    }

    BigRational::BigRational(const std::string& str) : value{str}
    {
    }

    BigRational::BigRational(int num) : value{num}
    {
    }

    BigRational::BigRational(long num) : value{num}
    {
    }

    BigRational::BigRational(long long num) : value{std::to_string(num)}
    {
    }

    BigRational::BigRational(const mpq_class& num) : value{num}
    {
    }

    BigRational::BigRational(int num, int den) : value{num, den}
    {
    }

    BigRational::BigRational(const BigInteger& num) : value(num.toString())
    {
    }
    BigRational::BigRational(const BigFloat& num) {
        std::string str = num.toString();
        value = mpq_class(str);
    }

    BigRational BigRational::operator+(const BigRational& other) const { return {value + other.value}; }
    BigRational BigRational::operator-(const BigRational& other) const { return {value - other.value}; }
    BigRational BigRational::operator*(const BigRational& other) const { return {value * other.value}; }
    BigRational BigRational::operator/(const BigRational& other) const { return {value / other.value}; }

    BigRational& BigRational::operator+=(const BigRational& other)
    {
        value += other.value;
        return *this;
    }

    BigRational& BigRational::operator-=(const BigRational& other)
    {
        value -= other.value;
        return *this;
    }

    BigRational& BigRational::operator*=(const BigRational& other)
    {
        value *= other.value;
        return *this;
    }

    BigRational& BigRational::operator/=(const BigRational& other)
    {
        value /= other.value;
        return *this;
    }

    bool BigRational::operator==(const BigRational& other) const { return value == other.value; }
    bool BigRational::operator!=(const BigRational& other) const { return value != other.value; }
    bool BigRational::operator<(const BigRational& other) const { return value < other.value; }
    bool BigRational::operator>(const BigRational& other) const { return value > other.value; }
    bool BigRational::operator<=(const BigRational& other) const { return value <= other.value; }
    bool BigRational::operator>=(const BigRational& other) const { return value >= other.value; }

    std::ostream& operator<<(std::ostream& os, const BigRational& num)
    {
        os << num.value.get_str();
        return os;
    }

    std::istream& operator>>(std::istream& is, BigRational& num)
    {
        std::string str;
        is >> str;
        num = BigRational{str};
        return is;
    }

    std::string BigRational::toString() const { return value.get_str(); }
} // mp
// usub
