#ifndef BIGFLOAT_H
#define BIGFLOAT_H

#include <gmpxx.h>

#include <iosfwd>
#include <string>

#include "unumber/integer/BigInteger.h"

namespace usub::mp {
    class BigFloat {
    public:
        BigFloat();

        BigFloat(const std::string &str, mp_bitcnt_t prec = 256);

        BigFloat(const char *str, mp_bitcnt_t prec = 256);

        BigFloat(double num, mp_bitcnt_t prec = 256);

        BigFloat(const mpf_class &num);

        BigFloat(const BigInteger &num, mp_bitcnt_t prec = 256);

        mp_bitcnt_t getPrecision() const noexcept { return precision_; }

        void setPrecision(mp_bitcnt_t prec);

        BigFloat operator+(const BigFloat &other) const;

        BigFloat operator-(const BigFloat &other) const;

        BigFloat operator*(const BigFloat &other) const;

        BigFloat operator/(const BigFloat &other) const;

        BigFloat &operator+=(const BigFloat &other);

        BigFloat &operator-=(const BigFloat &other);

        BigFloat &operator*=(const BigFloat &other);

        BigFloat &operator/=(const BigFloat &other);

        bool operator==(const BigFloat &other) const;

        bool operator!=(const BigFloat &other) const;

        bool operator<(const BigFloat &other) const;

        bool operator>(const BigFloat &other) const;

        bool operator<=(const BigFloat &other) const;

        bool operator>=(const BigFloat &other) const;

        friend std::ostream &operator<<(std::ostream &os, const BigFloat &num);

        friend std::istream &operator>>(std::istream &is, BigFloat &num);

        std::string toString() const;

        std::string toStringScientific() const;

        std::string toStringFixed(int digits) const;

    private:
        mpf_class value_{0};
        mp_bitcnt_t precision_{256};

        static mp_bitcnt_t maxPrec(mp_bitcnt_t a, mp_bitcnt_t b) noexcept { return (a > b) ? a : b; }

        void initWithPrecision(mp_bitcnt_t prec);

        void setFromString(const std::string &s);

        static mp_bitcnt_t dec_digits_from_bits(mp_bitcnt_t bits);

        static std::string toScientific10(const mpf_class &v, mp_bitcnt_t precBits);

        static std::string toFixed10(const mpf_class &v, int digits, mp_bitcnt_t workPrecBits);
    };
} // namespace usub::mp

#endif // BIGFLOAT_H
