#ifndef BIGMODULAR_H
#define BIGMODULAR_H

#include <gmpxx.h>

#include <iosfwd>
#include <string>

#include "unumber/integer/BigInteger.h"

namespace usub::mp {
    class BigModular {
    public:
        BigModular();

        BigModular(const BigInteger &val, const BigInteger &mod);

        BigModular operator+(const BigModular &other) const;

        BigModular operator-(const BigModular &other) const;

        BigModular operator*(const BigModular &other) const;

        BigModular operator/(const BigModular &other) const;

        BigModular inverse() const;

        BigModular &operator+=(const BigModular &other);

        BigModular &operator-=(const BigModular &other);

        BigModular &operator*=(const BigModular &other);

        BigModular &operator/=(const BigModular &other);

        bool operator==(const BigModular &other) const;

        bool operator!=(const BigModular &other) const;

        std::string toString() const;

        friend std::ostream &operator<<(std::ostream &os, const BigModular &num);

    private:
        mpz_class value_{0};
        mpz_class modulus_{1};

        static void requireSameModulus(const BigModular &a, const BigModular &b);

        static void requireValidModulus(const mpz_class &m);

        static mpz_class norm(const mpz_class &v, const mpz_class &m);

        BigModular(const mpz_class &v, const mpz_class &m);
    };
} // namespace usub::mp

#endif // BIGMODULAR_H
