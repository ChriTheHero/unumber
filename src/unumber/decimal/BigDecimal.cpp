#include "unumber/decimal/BigDecimal.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace usub::mp {
    static inline std::string trim_copy(const std::string &s) {
        auto is_space = [](unsigned char c) { return std::isspace(c) != 0; };
        size_t b = 0;
        while (b < s.size() && is_space(static_cast<unsigned char>(s[b]))) ++b;
        size_t e = s.size();
        while (e > b && is_space(static_cast<unsigned char>(s[e - 1]))) --e;
        return s.substr(b, e - b);
    }

    static inline void erase_underscores(std::string &s) {
        s.erase(std::remove(s.begin(), s.end(), '_'), s.end());
    }

    mpz_class BigDecimal::pow10(int exp) {
        if (exp < 0) throw std::invalid_argument("pow10: negative exp");
        mpz_class out;
        ::mpz_ui_pow_ui(out.get_mpz_t(), 10u, static_cast<unsigned long>(exp));
        return out;
    }

    void BigDecimal::requireSameScale(const BigDecimal &other) const {
        if (scale_ != other.scale_) {
            throw std::invalid_argument("BigDecimal: scale mismatch");
        }
    }

    mpz_class BigDecimal::divRound(const mpz_class &num, const mpz_class &den, Rounding mode) {
        if (den == 0) throw std::domain_error("division by zero");

        mpz_class n = num;
        mpz_class d = den;
        if (d < 0) {
            d = -d;
            n = -n;
        }

        switch (mode) {
            case Rounding::Trunc: {
                mpz_class q;
                ::mpz_tdiv_q(q.get_mpz_t(), n.get_mpz_t(), d.get_mpz_t());
                return q;
            }
            case Rounding::Floor: {
                mpz_class q;
                ::mpz_fdiv_q(q.get_mpz_t(), n.get_mpz_t(), d.get_mpz_t());
                return q;
            }
            case Rounding::Ceil: {
                mpz_class q;
                ::mpz_cdiv_q(q.get_mpz_t(), n.get_mpz_t(), d.get_mpz_t());
                return q;
            }
            case Rounding::HalfUp:
            case Rounding::HalfEven: {
                mpz_class q, r;
                ::mpz_tdiv_qr(q.get_mpz_t(), r.get_mpz_t(), n.get_mpz_t(), d.get_mpz_t());
                if (r == 0) return q;

                mpz_class abs_r = r;
                if (abs_r < 0) abs_r = -abs_r;

                mpz_class twice = abs_r * 2;
                const int cmp = ::mpz_cmp(twice.get_mpz_t(), d.get_mpz_t());

                if (cmp < 0) return q;

                if (cmp > 0) {
                    mpz_class res = q;
                    if (n >= 0) res += 1;
                    else res -= 1;
                    return res;
                }

                if (mode == Rounding::HalfUp) {
                    mpz_class res = q;
                    if (n >= 0) res += 1;
                    else res -= 1;
                    return res;
                }

                mpz_class abs_q = q;
                if (abs_q < 0) abs_q = -abs_q;

                const bool is_odd = (::mpz_tstbit(abs_q.get_mpz_t(), 0) != 0);
                if (!is_odd) return q;

                mpz_class res = q;
                if (n >= 0) res += 1;
                else res -= 1;
                return res;
            }
        }

        return 0;
    }

    mpz_class BigDecimal::divPow10Round(const mpz_class &v, int exp, Rounding mode) {
        if (exp < 0) throw std::invalid_argument("divPow10Round: negative exp");
        if (exp == 0) return v;
        return divRound(v, pow10(exp), mode);
    }

    BigDecimal::BigDecimal(mpz_class rawValue, int scale, bool)
        : value_(std::move(rawValue)), scale_(scale), scaleFactor_(pow10(scale)) {
        if (scale_ < 0) throw std::invalid_argument("scale must be >= 0");
    }

    BigDecimal::BigDecimal(int scale)
        : value_(0), scale_(scale), scaleFactor_(pow10(scale)) {
        if (scale_ < 0) throw std::invalid_argument("scale must be >= 0");
    }

    BigDecimal::BigDecimal(const std::string &str, int scale)
        : scale_(scale), scaleFactor_(pow10(scale)) {
        if (scale_ < 0) throw std::invalid_argument("scale must be >= 0");
        parseString(str);
    }

    BigDecimal::BigDecimal(const char *str, int scale)
        : BigDecimal(std::string(str ? str : ""), scale) {
    }

    BigDecimal::BigDecimal(int num, int scale)
        : scale_(scale), scaleFactor_(pow10(scale)) {
        if (scale_ < 0) throw std::invalid_argument("scale must be >= 0");
        value_ = mpz_class(std::to_string(num)) * scaleFactor_;
    }

    BigDecimal::BigDecimal(long num, int scale)
        : scale_(scale), scaleFactor_(pow10(scale)) {
        if (scale_ < 0) throw std::invalid_argument("scale must be >= 0");
        value_ = mpz_class(std::to_string(num)) * scaleFactor_;
    }

    BigDecimal::BigDecimal(long long num, int scale)
        : scale_(scale), scaleFactor_(pow10(scale)) {
        if (scale_ < 0) throw std::invalid_argument("scale must be >= 0");
        value_ = mpz_class(std::to_string(num)) * scaleFactor_;
    }

    BigDecimal::BigDecimal(double num, int scale)
        : scale_(scale), scaleFactor_(pow10(scale)) {
        if (scale_ < 0) throw std::invalid_argument("scale must be >= 0");
        if (!std::isfinite(num)) throw std::invalid_argument("double is not finite");

        std::ostringstream oss;
        oss.setf(std::ios::fixed);
        oss << std::setprecision(scale_) << num;
        parseString(oss.str());
    }

    mpq_class BigDecimal::toRational() const {
        mpq_class q(value_);
        q /= mpq_class(scaleFactor_);
        return q;
    }

    BigDecimal BigDecimal::abs() const {
        if (value_ >= 0) return *this;
        return {-value_, scale_, true};
    }

    BigDecimal BigDecimal::negate() const {
        if (value_ == 0) return *this;
        return {-value_, scale_, true};
    }

    void BigDecimal::parseString(const std::string &input) {
        std::string s = trim_copy(input);
        if (s.empty()) throw std::invalid_argument("empty string");

        int sign = 1;
        if (s[0] == '+') s.erase(0, 1);
        else if (s[0] == '-') {
            sign = -1;
            s.erase(0, 1);
        }

        s = trim_copy(s);
        if (s.empty()) throw std::invalid_argument("invalid number");

        int exp10 = 0; {
            size_t epos = s.find_first_of("eE");
            if (epos != std::string::npos) {
                std::string mant = s.substr(0, epos);
                std::string exps = trim_copy(s.substr(epos + 1));
                if (exps.empty()) throw std::invalid_argument("invalid exponent");

                size_t idx = 0;
                int esign = 1;
                if (exps[idx] == '+') ++idx;
                else if (exps[idx] == '-') {
                    esign = -1;
                    ++idx;
                }
                if (idx >= exps.size()) throw std::invalid_argument("invalid exponent");

                long long e = 0;
                for (; idx < exps.size(); ++idx) {
                    char c = exps[idx];
                    if (!std::isdigit(static_cast<unsigned char>(c))) throw std::invalid_argument("invalid exponent");
                    int d = c - '0';
                    if (e > (std::numeric_limits<long long>::max() - d) / 10)
                        throw std::invalid_argument(
                            "exponent too large");
                    e = e * 10 + d;
                }
                exp10 = static_cast<int>(esign * e);
                s = trim_copy(mant);
                if (s.empty()) throw std::invalid_argument("invalid mantissa");
            }
        }

        size_t dot = s.find('.');
        std::string intPart = (dot == std::string::npos) ? s : s.substr(0, dot);
        std::string fracPart = (dot == std::string::npos) ? "" : s.substr(dot + 1);

        if (intPart.empty()) intPart = "0";

        erase_underscores(intPart);
        erase_underscores(fracPart);

        auto only_digits = [](const std::string &p) {
            return std::ranges::all_of(p, [](unsigned char c) {
                return std::isdigit(c) != 0;
            });
        };
        if (!only_digits(intPart) || !only_digits(fracPart)) throw std::invalid_argument("invalid number");

        size_t nz = intPart.find_first_not_of('0');
        if (nz == std::string::npos) intPart = "0";
        else intPart.erase(0, nz);

        std::string digits = intPart + fracPart;
        if (digits.empty()) digits = "0";

        int fracLen = static_cast<int>(fracPart.size());

        if (exp10 != 0) {
            int newFrac = fracLen - exp10;
            if (newFrac < 0) {
                digits.append(static_cast<size_t>(-newFrac), '0');
                newFrac = 0;
            }
            fracLen = newFrac;
        }

        size_t nz2 = digits.find_first_not_of('0');
        std::string digitsTrim = (nz2 == std::string::npos) ? "0" : digits.substr(nz2);

        mpz_class unscaled(digitsTrim, 10);

        if (fracLen < scale_) {
            unscaled *= pow10(scale_ - fracLen);
        } else if (fracLen > scale_) {
            unscaled = divPow10Round(unscaled, fracLen - scale_, Rounding::HalfEven);
        }

        if (sign < 0 && unscaled != 0) unscaled = -unscaled;
        value_ = unscaled;
    }

    mpz_class BigDecimal::toScaleUnscaled(int targetScale, Rounding mode) const {
        if (targetScale < 0) throw std::invalid_argument("targetScale must be >= 0");
        if (targetScale == scale_) return value_;

        if (targetScale > scale_) {
            return value_ * pow10(targetScale - scale_);
        }
        return divPow10Round(value_, scale_ - targetScale, mode);
    }

    BigDecimal BigDecimal::rescaled(int newScale, Rounding mode) const {
        if (newScale < 0) throw std::invalid_argument("newScale must be >= 0");
        if (newScale == scale_) return *this;
        mpz_class nv = toScaleUnscaled(newScale, mode);
        return {nv, newScale, true};
    }

    void BigDecimal::setScale(int newScale, Rounding mode) {
        if (newScale < 0) throw std::invalid_argument("newScale must be >= 0");
        if (newScale == scale_) return;

        value_ = toScaleUnscaled(newScale, mode);
        scale_ = newScale;
        scaleFactor_ = pow10(scale_);
    }

    BigDecimal BigDecimal::stripTrailingZeros() const {
        if (value_ == 0) return {0, 0, true};

        mpz_class v = value_;
        int s = scale_;

        while (s > 0) {
            mpz_class q;
            ::mpz_tdiv_q_ui(q.get_mpz_t(), v.get_mpz_t(), 10u);
            mpz_class back = q * 10;
            if (back != v) break;
            v = q;
            --s;
        }

        return {v, s, true};
    }

    std::string BigDecimal::toString() const {
        if (scale_ < 0) throw std::logic_error("invalid scale");
        if (scale_ == 0) return value_.get_str();

        const bool neg = (value_ < 0);
        mpz_class av = value_;
        if (neg) av = -av;

        std::string s = av.get_str();

        if (av == 0) {
            return "0." + std::string(static_cast<size_t>(scale_), '0');
        }

        if (s.length() <= static_cast<size_t>(scale_)) {
            std::string out = "0." + std::string(static_cast<size_t>(scale_) - s.length(), '0') + s;
            return neg ? ("-" + out) : out;
        }

        const size_t cut = s.length() - static_cast<size_t>(scale_);
        std::string out = s.substr(0, cut) + "." + s.substr(cut);
        return neg ? ("-" + out) : out;
    }

    std::string BigDecimal::toString(int outScale, Rounding mode) const {
        return rescaled(outScale, mode).toString();
    }

    double BigDecimal::toDouble() const {
        mpf_class f(value_);
        f /= mpf_class(scaleFactor_);
        return f.get_d();
    }

    long long BigDecimal::toInt64Exact() const {
        mpz_class i = value_;
        if (scale_ != 0) {
            mpz_class rem;
            ::mpz_tdiv_r(rem.get_mpz_t(), value_.get_mpz_t(), scaleFactor_.get_mpz_t());
            if (rem != 0) throw std::invalid_argument("BigDecimal: not an integer");
            i = value_ / scaleFactor_;
        }

        const mpz_class mn(std::to_string(std::numeric_limits<long long>::min()));
        const mpz_class mx(std::to_string(std::numeric_limits<long long>::max()));
        if (i < mn || i > mx) throw std::out_of_range("BigDecimal: int64 out of range");

        return std::stoll(i.get_str());
    }

    unsigned long long BigDecimal::toUint64Exact() const {
        mpz_class i = value_;
        if (scale_ != 0) {
            mpz_class rem;
            ::mpz_tdiv_r(rem.get_mpz_t(), value_.get_mpz_t(), scaleFactor_.get_mpz_t());
            if (rem != 0) throw std::invalid_argument("BigDecimal: not an integer");
            i = value_ / scaleFactor_;
        }
        if (i < 0) throw std::out_of_range("BigDecimal: uint64 negative");

        const mpz_class mx(std::to_string(std::numeric_limits<unsigned long long>::max()));
        if (i > mx) throw std::out_of_range("BigDecimal: uint64 out of range");

        return std::stoull(i.get_str());
    }

    BigDecimal BigDecimal::add(const BigDecimal &other) const {
        requireSameScale(other);
        return {value_ + other.value_, scale_, true};
    }

    BigDecimal BigDecimal::sub(const BigDecimal &other) const {
        requireSameScale(other);
        return {value_ - other.value_, scale_, true};
    }

    BigDecimal BigDecimal::mul(const BigDecimal &other, Rounding mode) const {
        requireSameScale(other);
        mpz_class prod = value_ * other.value_;
        mpz_class res = divRound(prod, scaleFactor_, mode);
        return {res, scale_, true};
    }

    BigDecimal BigDecimal::div(const BigDecimal &other, Rounding mode) const {
        requireSameScale(other);
        if (other.value_ == 0) throw std::domain_error("division by zero");

        mpz_class num = value_ * scaleFactor_;
        mpz_class res = divRound(num, other.value_, mode);
        return {res, scale_, true};
    }

    BigDecimal &BigDecimal::operator+=(const BigDecimal &other) {
        requireSameScale(other);
        value_ += other.value_;
        return *this;
    }

    BigDecimal &BigDecimal::operator-=(const BigDecimal &other) {
        requireSameScale(other);
        value_ -= other.value_;
        return *this;
    }

    BigDecimal &BigDecimal::operator*=(const BigDecimal &other) {
        *this = mul(other, Rounding::HalfEven);
        return *this;
    }

    BigDecimal &BigDecimal::operator/=(const BigDecimal &other) {
        *this = div(other, Rounding::HalfEven);
        return *this;
    }

    BigDecimal BigDecimal::floor() const {
        if (scale_ == 0) return *this;

        mpz_class q;
        ::mpz_fdiv_q(q.get_mpz_t(), value_.get_mpz_t(), scaleFactor_.get_mpz_t());
        mpz_class nv = q * scaleFactor_;
        return {nv, scale_, true};
    }

    BigDecimal BigDecimal::ceil() const {
        if (scale_ == 0) return *this;

        mpz_class q;
        ::mpz_cdiv_q(q.get_mpz_t(), value_.get_mpz_t(), scaleFactor_.get_mpz_t());
        mpz_class nv = q * scaleFactor_;
        return {nv, scale_, true};
    }

    BigDecimal BigDecimal::round(int ndigits, Rounding mode) const {
        return rescaled(ndigits, mode);
    }

    BigDecimal BigDecimal::pow(int exp, Rounding mode) const {
        if (exp == 0) {
            return {scaleFactor_, scale_, true};
        }

        if (exp < 0) {
            BigDecimal pos = pow(-exp, mode);
            BigDecimal one(scaleFactor_, scale_, true);
            return one.div(pos, mode);
        }

        const bool neg = (value_ < 0);
        mpz_class av = value_;
        if (neg) av = -av;

        mpz_class p;
        ::mpz_pow_ui(p.get_mpz_t(), av.get_mpz_t(), static_cast<unsigned long>(exp));

        if (neg && (exp & 1)) p = -p;

        const long long denomExp = static_cast<long long>(scale_) * static_cast<long long>(exp - 1);
        if (denomExp < 0 || denomExp > std::numeric_limits<int>::max()) {
            throw std::invalid_argument("BigDecimal: pow exponent too large");
        }

        mpz_class res = (denomExp == 0) ? p : divPow10Round(p, static_cast<int>(denomExp), mode);
        return {res, scale_, true};
    }

    BigDecimal BigDecimal::sqrt(const BigDecimal &x, int outScale, Rounding mode) {
        if (outScale < 0) throw std::invalid_argument("sqrt: outScale must be >= 0");
        if (x.value_ < 0) throw std::domain_error("sqrt: negative");

        const int dec = outScale + 16;
        const int bits = static_cast<int>(dec * 3.3219280948873626) + 64;

        mpf_class f;
        f.set_prec(static_cast<mp_bitcnt_t>(bits));
        f = mpf_class(x.value_);
        f /= mpf_class(x.scaleFactor_);

        mpf_class r;
        r.set_prec(static_cast<mp_bitcnt_t>(bits));
        ::mpf_sqrt(r.get_mpf_t(), f.get_mpf_t());

        std::ostringstream oss;
        oss.setf(std::ios::fixed);
        oss << std::setprecision(outScale + 2) << r;
        BigDecimal tmp(oss.str(), outScale + 2);
        return tmp.rescaled(outScale, mode);
    }

    int BigDecimal::compare(const BigDecimal &other) const {
        requireSameScale(other);
        if (value_ < other.value_) return -1;
        if (value_ > other.value_) return 1;
        return 0;
    }

    BigDecimal BigDecimal::min(const BigDecimal &a, const BigDecimal &b) {
        a.requireSameScale(b);
        return (a.value_ <= b.value_) ? a : b;
    }

    BigDecimal BigDecimal::max(const BigDecimal &a, const BigDecimal &b) {
        a.requireSameScale(b);
        return (a.value_ >= b.value_) ? a : b;
    }

    BigDecimal BigDecimal::clamp(const BigDecimal &v, const BigDecimal &lo, const BigDecimal &hi) {
        v.requireSameScale(lo);
        v.requireSameScale(hi);
        if (v.value_ < lo.value_) return lo;
        if (v.value_ > hi.value_) return hi;
        return v;
    }

    std::size_t BigDecimal::hash() const {
        std::string s = value_.get_str();
        std::size_t h = 1469598103934665603ull;
        auto mix = [&](unsigned char c) {
            h ^= static_cast<std::size_t>(c);
            h *= 1099511628211ull;
        };
        mix(static_cast<unsigned char>(scale_ & 0xFF));
        mix(static_cast<unsigned char>((scale_ >> 8) & 0xFF));
        for (unsigned char c: s) mix(c);
        return h;
    }

    std::ostream &operator<<(std::ostream &os, const BigDecimal &num) {
        os << num.toString();
        return os;
    }
} // namespace usub::mp
