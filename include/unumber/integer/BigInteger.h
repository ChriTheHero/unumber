#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <gmpxx.h>

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

namespace usub::mp {
    class BigInteger {
    public:
        friend class BigModular;

        BigInteger();

        BigInteger(const std::string &str);

        BigInteger(const std::string &str, int base);

        BigInteger(const char *str);

        BigInteger(const char *str, int base);

        BigInteger(int num);

        BigInteger(long num);

        BigInteger(long long num);

        BigInteger(unsigned int num);

        BigInteger(unsigned long num);

        BigInteger(unsigned long long num);

        BigInteger(const mpz_class &num);

        static BigInteger fromString(const std::string &str, int base);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toString(int base) const;

        [[nodiscard]] const mpz_class &raw() const noexcept { return value_; }
        mpz_class &raw() noexcept { return value_; }

        [[nodiscard]] bool isZero() const noexcept { return value_ == 0; }
        [[nodiscard]] int sign() const noexcept { return (value_ > 0) - (value_ < 0); }

        [[nodiscard]] BigInteger abs() const;

        [[nodiscard]] BigInteger negate() const;

        explicit operator bool() const noexcept { return !isZero(); }

        BigInteger operator-() const { return negate(); }

        BigInteger &operator++();

        BigInteger operator++(int);

        BigInteger &operator--();

        BigInteger operator--(int);

        BigInteger operator+(const BigInteger &other) const;

        BigInteger operator-(const BigInteger &other) const;

        BigInteger operator*(const BigInteger &other) const;

        BigInteger operator/(const BigInteger &other) const;

        BigInteger operator%(const BigInteger &other) const;

        BigInteger &operator+=(const BigInteger &other);

        BigInteger &operator-=(const BigInteger &other);

        BigInteger &operator*=(const BigInteger &other);

        BigInteger &operator/=(const BigInteger &other);

        BigInteger &operator%=(const BigInteger &other);

        BigInteger operator&(const BigInteger &other) const;

        BigInteger operator|(const BigInteger &other) const;

        BigInteger operator^(const BigInteger &other) const;

        BigInteger operator~() const;

        BigInteger operator<<(std::size_t bits) const;

        BigInteger operator>>(std::size_t bits) const;

        BigInteger &operator&=(const BigInteger &other);

        BigInteger &operator|=(const BigInteger &other);

        BigInteger &operator^=(const BigInteger &other);

        BigInteger &operator<<=(std::size_t bits);

        BigInteger &operator>>=(std::size_t bits);

        bool operator==(const BigInteger &other) const;

        bool operator!=(const BigInteger &other) const;

        bool operator<(const BigInteger &other) const;

        bool operator>(const BigInteger &other) const;

        bool operator<=(const BigInteger &other) const;

        bool operator>=(const BigInteger &other) const;

        [[nodiscard]] int compare(const BigInteger &other) const;

        static std::pair<BigInteger, BigInteger> divmod(const BigInteger &a, const BigInteger &b);

        static BigInteger gcd(const BigInteger &a, const BigInteger &b);

        static BigInteger lcm(const BigInteger &a, const BigInteger &b);

        [[nodiscard]] BigInteger pow(std::uint64_t exp) const;

        [[nodiscard]] BigInteger pow_mod(const BigInteger &exp, const BigInteger &mod) const;

        [[nodiscard]] BigInteger pow_mod(std::uint64_t exp, const BigInteger &mod) const;

        [[nodiscard]] BigInteger mod(const BigInteger &modulus) const;

        [[nodiscard]] BigInteger inv_mod(const BigInteger &modulus) const;

        [[nodiscard]] bool is_probable_prime(int reps = 25) const;

        [[nodiscard]] BigInteger next_prime() const;

        [[nodiscard]] std::size_t bit_length() const;

        [[nodiscard]] std::size_t popcount() const;

        [[nodiscard]] bool bit_test(std::size_t bit) const;

        [[nodiscard]] BigInteger with_bit_set(std::size_t bit) const;

        [[nodiscard]] BigInteger with_bit_clear(std::size_t bit) const;

        void bit_set(std::size_t bit);

        void bit_clear(std::size_t bit);

        [[nodiscard]] long long toInt64Exact() const;

        [[nodiscard]] unsigned long long toUint64Exact() const;

        [[nodiscard]] std::vector<unsigned char> toBytesBE() const;

        [[nodiscard]] std::vector<unsigned char> toBytesLE() const;

        static BigInteger fromBytesBE(const std::vector<unsigned char> &bytes);

        static BigInteger fromBytesLE(const std::vector<unsigned char> &bytes);

        static BigInteger random_bits(std::size_t bits, std::uint64_t seed);

        static BigInteger random_range(const BigInteger &upperExclusive, std::uint64_t seed);

        void swap(BigInteger &other) noexcept;

        [[nodiscard]] std::size_t hash() const noexcept;

        friend std::ostream &operator<<(std::ostream &os, const BigInteger &num);

        friend std::istream &operator>>(std::istream &is, BigInteger &num);

    private:
        mpz_class value_{0};

        static void requireNonZeroDivisor(const BigInteger &b);

        static void requireNonNegative(const BigInteger &a, const char *what);

        static void requireValidBase(int base);

        explicit BigInteger(mpz_class &&num) : value_(std::move(num)) {
        }
    };
} // namespace usub::mp

namespace std {
    template<>
    struct hash<usub::mp::BigInteger> {
        size_t operator()(const usub::mp::BigInteger &v) const noexcept { return v.hash(); }
    };
} // namespace std

#endif // BIGINTEGER_H
