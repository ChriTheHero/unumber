#include "unumber/float/BigFloat.h"

#include <cctype>
#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>

namespace usub::mp {
    static std::string trim_copy(const std::string &s) {
        auto is_space = [](unsigned char c) { return std::isspace(c) != 0; };
        std::size_t b = 0;
        while (b < s.size() && is_space(static_cast<unsigned char>(s[b]))) ++b;
        std::size_t e = s.size();
        while (e > b && is_space(static_cast<unsigned char>(s[e - 1]))) --e;
        return s.substr(b, e - b);
    }

    void BigFloat::initWithPrecision(mp_bitcnt_t prec) {
        if (prec == 0) throw std::invalid_argument("BigFloat: precision is zero");
        precision_ = prec;
        value_.set_prec(precision_);
    }

    void BigFloat::setFromString(const std::string &s) {
        const std::string t = trim_copy(s);
        if (t.empty()) throw std::invalid_argument("BigFloat: empty string");
        const int rc = ::mpf_set_str(value_.get_mpf_t(), t.c_str(), 10);
        if (rc != 0) throw std::invalid_argument("BigFloat: invalid string");
    }

    mp_bitcnt_t BigFloat::dec_digits_from_bits(mp_bitcnt_t bits) {
        const long double x = static_cast<long double>(bits) * 0.3010299956639811952137389L;
        const long double d = std::ceil(x) + 2.0L;
        if (d < 2.0L) return static_cast<mp_bitcnt_t>(2);
        if (d > 1000000.0L) return static_cast<mp_bitcnt_t>(1000000);
        return static_cast<mp_bitcnt_t>(d);
    }

    std::string BigFloat::toScientific10(const mpf_class &v, mp_bitcnt_t precBits) {
        if (v == 0) return "0";

        mp_exp_t exp = 0;
        const mp_bitcnt_t ndigits = dec_digits_from_bits(precBits);

        char *mant = ::mpf_get_str(nullptr, &exp, 10, static_cast<std::size_t>(ndigits), v.get_mpf_t());
        if (!mant) throw std::runtime_error("BigFloat: mpf_get_str failed");

        std::string m(mant);

        void (*freefunc)(void *, std::size_t) = nullptr;
        ::mp_get_memory_functions(nullptr, nullptr, &freefunc);
        if (freefunc) freefunc(mant, m.size() + 1);

        bool neg = false;
        if (!m.empty() && m[0] == '-') {
            neg = true;
            m.erase(0, 1);
        }

        while (m.size() > 1 && m.back() == '0') m.pop_back();
        if (m == "0") return "0";

        std::string out;
        if (neg) out.push_back('-');

        if (m.size() == 1) {
            out.push_back(m[0]);
        } else {
            out.push_back(m[0]);
            out.push_back('.');
            out.append(m.substr(1));
        }

        const long long e10 = static_cast<long long>(exp) - 1LL;
        out.push_back('e');
        out.append(std::to_string(e10));
        return out;
    }

    static mpz_class pow10_z(int k) {
        mpz_class out;
        ::mpz_ui_pow_ui(out.get_mpz_t(), 10u, static_cast<unsigned long>(k));
        return out;
    }

    std::string BigFloat::toFixed10(const mpf_class &v, int digits, mp_bitcnt_t workPrecBits) {
        if (digits < 0) throw std::invalid_argument("BigFloat: digits < 0");
        if (v == 0) {
            if (digits == 0) return "0";
            return std::string("0.") + std::string(static_cast<std::size_t>(digits), '0');
        }

        mpf_class scaled;
        scaled.set_prec(workPrecBits);
        scaled = v;

        mpz_class p10 = pow10_z(digits);
        mpf_class factor;
        factor.set_prec(workPrecBits);
        ::mpf_set_z(factor.get_mpf_t(), p10.get_mpz_t());

        ::mpf_mul(scaled.get_mpf_t(), scaled.get_mpf_t(), factor.get_mpf_t());

        mpz_class i;
        ::mpz_set_f(i.get_mpz_t(), scaled.get_mpf_t());

        bool neg = false;
        if (i < 0) {
            neg = true;
            i = -i;
        }

        std::string s = i.get_str();

        std::string out;
        if (neg) out.push_back('-');

        if (digits == 0) {
            out.append(s);
            return out;
        }

        if (s.size() <= static_cast<std::size_t>(digits)) {
            out.append("0.");
            out.append(std::string(static_cast<std::size_t>(digits) - s.size(), '0'));
            out.append(s);
            return out;
        }

        const std::size_t cut = s.size() - static_cast<std::size_t>(digits);
        out.append(s.substr(0, cut));
        out.push_back('.');
        out.append(s.substr(cut));
        return out;
    }

    BigFloat::BigFloat() {
        initWithPrecision(256);
        ::mpf_set_ui(value_.get_mpf_t(), 0u);
    }

    BigFloat::BigFloat(const std::string &str, mp_bitcnt_t prec) {
        initWithPrecision(prec);
        setFromString(str);
    }

    BigFloat::BigFloat(const char *str, mp_bitcnt_t prec) {
        initWithPrecision(prec);
        setFromString(std::string(str ? str : ""));
    }

    BigFloat::BigFloat(double num, mp_bitcnt_t prec) {
        initWithPrecision(prec);
        ::mpf_set_d(value_.get_mpf_t(), num);
    }

    BigFloat::BigFloat(const mpf_class &num) : value_(num), precision_(num.get_prec()) {
    }

    BigFloat::BigFloat(const BigInteger &num, mp_bitcnt_t prec) {
        initWithPrecision(prec);
        ::mpf_set_z(value_.get_mpf_t(), num.raw().get_mpz_t());
    }

    void BigFloat::setPrecision(mp_bitcnt_t prec) {
        if (prec == 0) throw std::invalid_argument("BigFloat: precision is zero");
        if (prec == precision_) return;
        mpf_class tmp = value_;
        precision_ = prec;
        value_.set_prec(precision_);
        ::mpf_set(value_.get_mpf_t(), tmp.get_mpf_t());
    }

    BigFloat BigFloat::operator+(const BigFloat &other) const {
        BigFloat r;
        r.initWithPrecision(maxPrec(precision_, other.precision_));
        ::mpf_add(r.value_.get_mpf_t(), value_.get_mpf_t(), other.value_.get_mpf_t());
        return r;
    }

    BigFloat BigFloat::operator-(const BigFloat &other) const {
        BigFloat r;
        r.initWithPrecision(maxPrec(precision_, other.precision_));
        ::mpf_sub(r.value_.get_mpf_t(), value_.get_mpf_t(), other.value_.get_mpf_t());
        return r;
    }

    BigFloat BigFloat::operator*(const BigFloat &other) const {
        BigFloat r;
        r.initWithPrecision(maxPrec(precision_, other.precision_));
        ::mpf_mul(r.value_.get_mpf_t(), value_.get_mpf_t(), other.value_.get_mpf_t());
        return r;
    }

    BigFloat BigFloat::operator/(const BigFloat &other) const {
        BigFloat r;
        r.initWithPrecision(maxPrec(precision_, other.precision_));
        ::mpf_div(r.value_.get_mpf_t(), value_.get_mpf_t(), other.value_.get_mpf_t());
        return r;
    }

    BigFloat &BigFloat::operator+=(const BigFloat &other) {
        if (other.precision_ > precision_) setPrecision(other.precision_);
        ::mpf_add(value_.get_mpf_t(), value_.get_mpf_t(), other.value_.get_mpf_t());
        return *this;
    }

    BigFloat &BigFloat::operator-=(const BigFloat &other) {
        if (other.precision_ > precision_) setPrecision(other.precision_);
        ::mpf_sub(value_.get_mpf_t(), value_.get_mpf_t(), other.value_.get_mpf_t());
        return *this;
    }

    BigFloat &BigFloat::operator*=(const BigFloat &other) {
        if (other.precision_ > precision_) setPrecision(other.precision_);
        ::mpf_mul(value_.get_mpf_t(), value_.get_mpf_t(), other.value_.get_mpf_t());
        return *this;
    }

    BigFloat &BigFloat::operator/=(const BigFloat &other) {
        if (other.precision_ > precision_) setPrecision(other.precision_);
        ::mpf_div(value_.get_mpf_t(), value_.get_mpf_t(), other.value_.get_mpf_t());
        return *this;
    }

    bool BigFloat::operator==(const BigFloat &other) const { return value_ == other.value_; }
    bool BigFloat::operator!=(const BigFloat &other) const { return value_ != other.value_; }
    bool BigFloat::operator<(const BigFloat &other) const { return value_ < other.value_; }
    bool BigFloat::operator>(const BigFloat &other) const { return value_ > other.value_; }
    bool BigFloat::operator<=(const BigFloat &other) const { return value_ <= other.value_; }
    bool BigFloat::operator>=(const BigFloat &other) const { return value_ >= other.value_; }

    std::string BigFloat::toStringScientific() const {
        return toScientific10(value_, precision_);
    }

    std::string BigFloat::toStringFixed(int digits) const {
        const mp_bitcnt_t work = static_cast<mp_bitcnt_t>(precision_ + static_cast<mp_bitcnt_t>(64));
        return toFixed10(value_, digits, work);
    }

    std::string BigFloat::toString() const {
        const mp_bitcnt_t digs = dec_digits_from_bits(precision_);
        const int fixed = static_cast<int>(digs > 0 ? digs - 2 : 32);
        return toStringFixed(fixed);
    }

    std::ostream &operator<<(std::ostream &os, const BigFloat &num) {
        os << num.toString();
        return os;
    }

    std::istream &operator>>(std::istream &is, BigFloat &num) {
        std::string s;
        is >> s;
        num = BigFloat(s, num.precision_);
        return is;
    }
} // namespace usub::mp