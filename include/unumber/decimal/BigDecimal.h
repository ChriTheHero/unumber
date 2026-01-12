#ifndef BIGDECIMAL_H
#define BIGDECIMAL_H

#include <gmpxx.h>
#include <iosfwd>
#include <string>
#include <cstddef>

namespace usub::mp {
    class BigDecimal {
    public:
        enum class Rounding : unsigned char {
            Trunc,
            Floor,
            Ceil,
            HalfUp,
            HalfEven
        };

        explicit BigDecimal(int scale = 4);

        BigDecimal(const std::string &str, int scale = 4);

        BigDecimal(const char *str, int scale = 4);

        BigDecimal(int num, int scale = 4);

        BigDecimal(long num, int scale = 4);

        BigDecimal(long long num, int scale = 4);

        BigDecimal(double num, int scale = 4);

        [[nodiscard]] int getScale() const noexcept { return scale_; }
        [[nodiscard]] const mpz_class &unscaled() const noexcept { return value_; }

        [[nodiscard]] mpq_class toRational() const;

        [[nodiscard]] bool isZero() const noexcept { return value_ == 0; }
        [[nodiscard]] int sign() const noexcept { return (value_ > 0) - (value_ < 0); }

        [[nodiscard]] BigDecimal abs() const;

        [[nodiscard]] BigDecimal negate() const;

        BigDecimal operator-() const { return negate(); }

        [[nodiscard]] BigDecimal rescaled(int newScale, Rounding mode = Rounding::HalfEven) const;

        [[nodiscard]] BigDecimal quantize(int newScale, Rounding mode = Rounding::HalfEven) const {
            return rescaled(newScale, mode);
        }

        void setScale(int newScale, Rounding mode = Rounding::HalfEven);

        [[nodiscard]] BigDecimal stripTrailingZeros() const;

        [[nodiscard]] BigDecimal normalize() const { return stripTrailingZeros(); }

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toString(int outScale, Rounding mode = Rounding::HalfEven) const;

        [[nodiscard]] std::string toPlainString() const { return toString(); }

        [[nodiscard]] double toDouble() const;

        [[nodiscard]] long long toInt64Exact() const;

        [[nodiscard]] unsigned long long toUint64Exact() const;

        [[nodiscard]] BigDecimal add(const BigDecimal &other) const;

        [[nodiscard]] BigDecimal sub(const BigDecimal &other) const;

        [[nodiscard]] BigDecimal mul(const BigDecimal &other, Rounding mode = Rounding::HalfEven) const;

        [[nodiscard]] BigDecimal div(const BigDecimal &other, Rounding mode = Rounding::HalfEven) const;

        BigDecimal operator+(const BigDecimal &other) const { return add(other); }
        BigDecimal operator-(const BigDecimal &other) const { return sub(other); }
        BigDecimal operator*(const BigDecimal &other) const { return mul(other); }
        BigDecimal operator/(const BigDecimal &other) const { return div(other); }

        BigDecimal &operator+=(const BigDecimal &other);

        BigDecimal &operator-=(const BigDecimal &other);

        BigDecimal &operator*=(const BigDecimal &other);

        BigDecimal &operator/=(const BigDecimal &other);

        [[nodiscard]] BigDecimal floor() const;

        [[nodiscard]] BigDecimal ceil() const;

        [[nodiscard]] BigDecimal round(int ndigits, Rounding mode = Rounding::HalfEven) const;

        [[nodiscard]] BigDecimal trunc(int ndigits) const { return round(ndigits, Rounding::Trunc); }

        [[nodiscard]] BigDecimal pow(int exp, Rounding mode = Rounding::HalfEven) const;

        static BigDecimal sqrt(const BigDecimal &x, int outScale, Rounding mode = Rounding::HalfEven);

        [[nodiscard]] int compare(const BigDecimal &other) const;

        bool operator==(const BigDecimal &other) const { return compare(other) == 0; }
        bool operator!=(const BigDecimal &other) const { return compare(other) != 0; }
        bool operator<(const BigDecimal &other) const { return compare(other) < 0; }
        bool operator>(const BigDecimal &other) const { return compare(other) > 0; }
        bool operator<=(const BigDecimal &other) const { return compare(other) <= 0; }
        bool operator>=(const BigDecimal &other) const { return compare(other) >= 0; }

        static BigDecimal min(const BigDecimal &a, const BigDecimal &b);

        static BigDecimal max(const BigDecimal &a, const BigDecimal &b);

        static BigDecimal clamp(const BigDecimal &v, const BigDecimal &lo, const BigDecimal &hi);

        [[nodiscard]] std::size_t hash() const;

        friend std::ostream &operator<<(std::ostream &os, const BigDecimal &num);

    private:
        mpz_class value_{0};
        int scale_{0};
        mpz_class scaleFactor_{1};

        static mpz_class pow10(int exp);

        static mpz_class divRound(const mpz_class &num, const mpz_class &den, Rounding mode);

        static mpz_class divPow10Round(const mpz_class &v, int exp, Rounding mode);

        [[nodiscard]] mpz_class toScaleUnscaled(int targetScale, Rounding mode) const;

        void parseString(const std::string &str);

        void requireSameScale(const BigDecimal &other) const;

        BigDecimal(mpz_class rawValue, int scale, bool /*rawTag*/);
    };
} // namespace usub::mp

namespace std {
    template<>
    struct hash<usub::mp::BigDecimal> {
        size_t operator()(const usub::mp::BigDecimal &v) const noexcept {
            return v.hash();
        }
    };
} // namespace std

#endif // BIGDECIMAL_H
