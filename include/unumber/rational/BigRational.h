//
// Created by Kirill Zhukov on 29.03.2025.
//

#ifndef BIGRATIONAL_H
#define BIGRATIONAL_H

#include <gmpxx.h>
#include <iostream>
#include <string>

#include "unumber/float/BigFloat.h"
#include "unumber/integer/BigInteger.h"

namespace usub::mp
{
    class BigRational
    {
    public:
        BigRational();
        BigRational(const std::string& str);
        BigRational(int num);
        BigRational(long num);
        BigRational(long long num);
        BigRational(const mpq_class& num);
        BigRational(int num, int den);
        BigRational(const BigInteger& num);
        BigRational(const BigFloat& num);

        BigRational operator+(const BigRational& other) const;
        BigRational operator-(const BigRational& other) const;
        BigRational operator*(const BigRational& other) const;
        BigRational operator/(const BigRational& other) const;

        BigRational& operator+=(const BigRational& other);
        BigRational& operator-=(const BigRational& other);
        BigRational& operator*=(const BigRational& other);
        BigRational& operator/=(const BigRational& other);

        bool operator==(const BigRational& other) const;
        bool operator!=(const BigRational& other) const;
        bool operator<(const BigRational& other) const;
        bool operator>(const BigRational& other) const;
        bool operator<=(const BigRational& other) const;
        bool operator>=(const BigRational& other) const;

        friend std::ostream& operator<<(std::ostream& os, const BigRational& num);
        friend std::istream& operator>>(std::istream& is, BigRational& num);

        std::string toString() const;
    private:
        mpq_class value;
    };
} // mp
// usub

#endif //BIGRATIONAL_H
