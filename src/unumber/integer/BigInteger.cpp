#include "unumber/integer/BigInteger.h"

#include <algorithm>
#include <cctype>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>

namespace usub::mp {
    static std::string trim_copy(const std::string &s) {
        auto is_space = [](unsigned char c) { return std::isspace(c) != 0; };
        std::size_t b = 0;
        while (b < s.size() && is_space(static_cast<unsigned char>(s[b]))) ++b;
        std::size_t e = s.size();
        while (e > b && is_space(static_cast<unsigned char>(s[e - 1]))) --e;
        return s.substr(b, e - b);
    }

    void BigInteger::requireValidBase(int base) {
        if (base != 0 && (base < 2 || base > 62)) {
            throw std::invalid_argument("BigInteger: invalid base");
        }
    }

    void BigInteger::requireNonZeroDivisor(const BigInteger &b) {
        if (b.value_ == 0) {
            throw std::domain_error("BigInteger: division by zero");
        }
    }

    void BigInteger::requireNonNegative(const BigInteger &a, const char *what) {
        if (a.value_ < 0) {
            throw std::invalid_argument(std::string("BigInteger: ") + what + " requires non-negative");
        }
    }

    BigInteger::BigInteger() = default;

    BigInteger::BigInteger(const std::string &str) : value_(str) {
    }

    BigInteger::BigInteger(const std::string &str, int base) {
        requireValidBase(base);
        value_ = mpz_class(str, base);
    }

    BigInteger::BigInteger(const char *str) : value_(str ? str : "0") {
    }

    BigInteger::BigInteger(const char *str, int base) {
        requireValidBase(base);
        value_ = mpz_class(std::string(str ? str : "0"), base);
    }

    BigInteger::BigInteger(int num) : value_(num) {
    }

    BigInteger::BigInteger(long num) : value_(num) {
    }

    BigInteger::BigInteger(long long num) : value_(std::to_string(num)) {
    }

    BigInteger::BigInteger(unsigned int num) : value_(static_cast<unsigned long>(num)) {
    }

    BigInteger::BigInteger(unsigned long num) : value_(num) {
    }

    BigInteger::BigInteger(unsigned long long num) : value_(std::to_string(num)) {
    }

    BigInteger::BigInteger(const mpz_class &num) : value_(num) {
    }

    BigInteger BigInteger::fromString(const std::string &str, int base) {
        return {str, base};
    }

    std::string BigInteger::toString() const {
        return value_.get_str();
    }

    std::string BigInteger::toString(int base) const {
        requireValidBase(base);
        return value_.get_str(base);
    }

    BigInteger BigInteger::abs() const {
        if (value_ >= 0) return *this;
        return BigInteger(-value_);
    }

    BigInteger BigInteger::negate() const {
        if (value_ == 0) return *this;
        return BigInteger(-value_);
    }

    BigInteger &BigInteger::operator++() {
        value_ += 1;
        return *this;
    }

    BigInteger BigInteger::operator++(int) {
        BigInteger tmp(*this);
        value_ += 1;
        return tmp;
    }

    BigInteger &BigInteger::operator--() {
        value_ -= 1;
        return *this;
    }

    BigInteger BigInteger::operator--(int) {
        BigInteger tmp(*this);
        value_ -= 1;
        return tmp;
    }

    BigInteger BigInteger::operator+(const BigInteger &other) const { return BigInteger(value_ + other.value_); }
    BigInteger BigInteger::operator-(const BigInteger &other) const { return BigInteger(value_ - other.value_); }
    BigInteger BigInteger::operator*(const BigInteger &other) const { return BigInteger(value_ * other.value_); }

    BigInteger BigInteger::operator/(const BigInteger &other) const {
        requireNonZeroDivisor(other);
        return BigInteger(value_ / other.value_);
    }

    BigInteger BigInteger::operator%(const BigInteger &other) const {
        requireNonZeroDivisor(other);
        return BigInteger(value_ % other.value_);
    }

    BigInteger &BigInteger::operator+=(const BigInteger &other) {
        value_ += other.value_;
        return *this;
    }

    BigInteger &BigInteger::operator-=(const BigInteger &other) {
        value_ -= other.value_;
        return *this;
    }

    BigInteger &BigInteger::operator*=(const BigInteger &other) {
        value_ *= other.value_;
        return *this;
    }

    BigInteger &BigInteger::operator/=(const BigInteger &other) {
        requireNonZeroDivisor(other);
        value_ /= other.value_;
        return *this;
    }

    BigInteger &BigInteger::operator%=(const BigInteger &other) {
        requireNonZeroDivisor(other);
        value_ %= other.value_;
        return *this;
    }

    BigInteger BigInteger::operator&(const BigInteger &other) const {
        mpz_class out;
        ::mpz_and(out.get_mpz_t(), value_.get_mpz_t(), other.value_.get_mpz_t());
        return BigInteger(std::move(out));
    }

    BigInteger BigInteger::operator|(const BigInteger &other) const {
        mpz_class out;
        ::mpz_ior(out.get_mpz_t(), value_.get_mpz_t(), other.value_.get_mpz_t());
        return BigInteger(std::move(out));
    }

    BigInteger BigInteger::operator^(const BigInteger &other) const {
        mpz_class out;
        ::mpz_xor(out.get_mpz_t(), value_.get_mpz_t(), other.value_.get_mpz_t());
        return BigInteger(std::move(out));
    }

    BigInteger BigInteger::operator~() const {
        mpz_class out;
        ::mpz_com(out.get_mpz_t(), value_.get_mpz_t());
        return BigInteger(std::move(out));
    }

    BigInteger BigInteger::operator<<(std::size_t bits) const {
        mpz_class out;
        ::mpz_mul_2exp(out.get_mpz_t(), value_.get_mpz_t(), static_cast<mp_bitcnt_t>(bits));
        return BigInteger(std::move(out));
    }

    BigInteger BigInteger::operator>>(std::size_t bits) const {
        mpz_class out;
        ::mpz_fdiv_q_2exp(out.get_mpz_t(), value_.get_mpz_t(), static_cast<mp_bitcnt_t>(bits));
        return BigInteger(std::move(out));
    }

    BigInteger &BigInteger::operator&=(const BigInteger &other) {
        ::mpz_and(value_.get_mpz_t(), value_.get_mpz_t(), other.value_.get_mpz_t());
        return *this;
    }

    BigInteger &BigInteger::operator|=(const BigInteger &other) {
        ::mpz_ior(value_.get_mpz_t(), value_.get_mpz_t(), other.value_.get_mpz_t());
        return *this;
    }

    BigInteger &BigInteger::operator^=(const BigInteger &other) {
        ::mpz_xor(value_.get_mpz_t(), value_.get_mpz_t(), other.value_.get_mpz_t());
        return *this;
    }

    BigInteger &BigInteger::operator<<=(std::size_t bits) {
        ::mpz_mul_2exp(value_.get_mpz_t(), value_.get_mpz_t(), static_cast<mp_bitcnt_t>(bits));
        return *this;
    }

    BigInteger &BigInteger::operator>>=(std::size_t bits) {
        ::mpz_fdiv_q_2exp(value_.get_mpz_t(), value_.get_mpz_t(), static_cast<mp_bitcnt_t>(bits));
        return *this;
    }

    bool BigInteger::operator==(const BigInteger &other) const { return value_ == other.value_; }
    bool BigInteger::operator!=(const BigInteger &other) const { return value_ != other.value_; }
    bool BigInteger::operator<(const BigInteger &other) const { return value_ < other.value_; }
    bool BigInteger::operator>(const BigInteger &other) const { return value_ > other.value_; }
    bool BigInteger::operator<=(const BigInteger &other) const { return value_ <= other.value_; }
    bool BigInteger::operator>=(const BigInteger &other) const { return value_ >= other.value_; }

    int BigInteger::compare(const BigInteger &other) const {
        const int c = ::mpz_cmp(value_.get_mpz_t(), other.value_.get_mpz_t());
        if (c < 0) return -1;
        if (c > 0) return 1;
        return 0;
    }

    std::pair<BigInteger, BigInteger> BigInteger::divmod(const BigInteger &a, const BigInteger &b) {
        requireNonZeroDivisor(b);
        mpz_class q, r;
        ::mpz_tdiv_qr(q.get_mpz_t(), r.get_mpz_t(), a.value_.get_mpz_t(), b.value_.get_mpz_t());
        return {BigInteger(std::move(q)), BigInteger(std::move(r))};
    }

    BigInteger BigInteger::gcd(const BigInteger &a, const BigInteger &b) {
        mpz_class g;
        ::mpz_gcd(g.get_mpz_t(), a.value_.get_mpz_t(), b.value_.get_mpz_t());
        return BigInteger(std::move(g));
    }

    BigInteger BigInteger::lcm(const BigInteger &a, const BigInteger &b) {
        if (a.value_ == 0 || b.value_ == 0) return {0};
        mpz_class g;
        ::mpz_gcd(g.get_mpz_t(), a.value_.get_mpz_t(), b.value_.get_mpz_t());

        mpz_class aa = a.value_;
        if (aa < 0) aa = -aa;

        mpz_class bb = b.value_;
        if (bb < 0) bb = -bb;

        mpz_class t = aa / g;
        t *= bb;
        return BigInteger(std::move(t));
    }

    BigInteger BigInteger::pow(std::uint64_t exp) const {
        if (exp == 0) return {1};
        if (value_ == 0) return {0};

        BigInteger base(*this);
        BigInteger result(1);

        std::uint64_t e = exp;
        while (e != 0) {
            if ((e & 1u) != 0u) result *= base;
            e >>= 1u;
            if (e != 0) base *= base;
        }
        return result;
    }

    BigInteger BigInteger::pow_mod(const BigInteger &exp, const BigInteger &mod) const {
        requireNonZeroDivisor(mod);
        BigInteger m = mod;
        if (m.value_ < 0) m.value_ = -m.value_;

        mpz_class out;
        ::mpz_powm(out.get_mpz_t(), value_.get_mpz_t(), exp.value_.get_mpz_t(), m.value_.get_mpz_t());
        return BigInteger(std::move(out));
    }

    BigInteger BigInteger::pow_mod(std::uint64_t exp, const BigInteger &mod) const {
        return pow_mod(BigInteger(std::to_string(exp)), mod);
    }

    BigInteger BigInteger::mod(const BigInteger &modulus) const {
        requireNonZeroDivisor(modulus);
        BigInteger m = modulus;
        if (m.value_ < 0) m.value_ = -m.value_;

        mpz_class r;
        ::mpz_mod(r.get_mpz_t(), value_.get_mpz_t(), m.value_.get_mpz_t());
        return BigInteger(std::move(r));
    }

    BigInteger BigInteger::inv_mod(const BigInteger &modulus) const {
        requireNonZeroDivisor(modulus);
        BigInteger m = modulus;
        if (m.value_ < 0) m.value_ = -m.value_;

        mpz_class a;
        ::mpz_mod(a.get_mpz_t(), value_.get_mpz_t(), m.value_.get_mpz_t());

        mpz_class inv;
        const int ok = ::mpz_invert(inv.get_mpz_t(), a.get_mpz_t(), m.value_.get_mpz_t());
        if (ok == 0) {
            throw std::domain_error("BigInteger: inverse does not exist");
        }
        return BigInteger(std::move(inv));
    }

    bool BigInteger::is_probable_prime(int reps) const {
        if (reps < 1) reps = 1;
        const int r = ::mpz_probab_prime_p(value_.get_mpz_t(), reps);
        return r != 0;
    }

    BigInteger BigInteger::next_prime() const {
        BigInteger n(*this);
        if (n.value_ < 0) n.value_ = -n.value_;
        mpz_class out;
        ::mpz_nextprime(out.get_mpz_t(), n.value_.get_mpz_t());
        return BigInteger(std::move(out));
    }

    std::size_t BigInteger::bit_length() const {
        requireNonNegative(*this, "bit_length");
        if (value_ == 0) return 0;
        return static_cast<std::size_t>(::mpz_sizeinbase(value_.get_mpz_t(), 2));
    }

    std::size_t BigInteger::popcount() const {
        requireNonNegative(*this, "popcount");
        return static_cast<std::size_t>(::mpz_popcount(value_.get_mpz_t()));
    }

    bool BigInteger::bit_test(std::size_t bit) const {
        requireNonNegative(*this, "bit_test");
        return ::mpz_tstbit(value_.get_mpz_t(), static_cast<mp_bitcnt_t>(bit)) != 0;
    }

    BigInteger BigInteger::with_bit_set(std::size_t bit) const {
        requireNonNegative(*this, "with_bit_set");
        mpz_class out = value_;
        ::mpz_setbit(out.get_mpz_t(), static_cast<mp_bitcnt_t>(bit));
        return BigInteger(std::move(out));
    }

    BigInteger BigInteger::with_bit_clear(std::size_t bit) const {
        requireNonNegative(*this, "with_bit_clear");
        mpz_class out = value_;
        ::mpz_clrbit(out.get_mpz_t(), static_cast<mp_bitcnt_t>(bit));
        return BigInteger(std::move(out));
    }

    void BigInteger::bit_set(std::size_t bit) {
        requireNonNegative(*this, "bit_set");
        ::mpz_setbit(value_.get_mpz_t(), static_cast<mp_bitcnt_t>(bit));
    }

    void BigInteger::bit_clear(std::size_t bit) {
        requireNonNegative(*this, "bit_clear");
        ::mpz_clrbit(value_.get_mpz_t(), static_cast<mp_bitcnt_t>(bit));
    }

    long long BigInteger::toInt64Exact() const {
        const mpz_class mn(std::to_string(std::numeric_limits<long long>::min()));
        const mpz_class mx(std::to_string(std::numeric_limits<long long>::max()));
        if (value_ < mn || value_ > mx) throw std::out_of_range("BigInteger: int64 out of range");
        return std::stoll(value_.get_str());
    }

    unsigned long long BigInteger::toUint64Exact() const {
        if (value_ < 0) throw std::out_of_range("BigInteger: uint64 negative");
        const mpz_class mx(std::to_string(std::numeric_limits<unsigned long long>::max()));
        if (value_ > mx) throw std::out_of_range("BigInteger: uint64 out of range");
        return std::stoull(value_.get_str());
    }

    std::vector<unsigned char> BigInteger::toBytesBE() const {
        requireNonNegative(*this, "toBytesBE");
        if (value_ == 0) return {static_cast<unsigned char>(0)};

        std::size_t count = 0;
        const int order = 1;
        const std::size_t size = 1;
        const int endian = 1;
        const std::size_t nails = 0;

        (void) ::mpz_export(nullptr, &count, order, size, endian, nails, value_.get_mpz_t());
        std::vector<unsigned char> out(count);
        (void) ::mpz_export(out.data(), &count, order, size, endian, nails, value_.get_mpz_t());
        out.resize(count);
        if (out.empty()) out.push_back(static_cast<unsigned char>(0));
        return out;
    }

    std::vector<unsigned char> BigInteger::toBytesLE() const {
        requireNonNegative(*this, "toBytesLE");
        if (value_ == 0) return {static_cast<unsigned char>(0)};

        std::size_t count = 0;
        const int order = -1;
        const std::size_t size = 1;
        const int endian = 1;
        const std::size_t nails = 0;

        (void) ::mpz_export(nullptr, &count, order, size, endian, nails, value_.get_mpz_t());
        std::vector<unsigned char> out(count);
        (void) ::mpz_export(out.data(), &count, order, size, endian, nails, value_.get_mpz_t());
        out.resize(count);
        if (out.empty()) out.push_back(static_cast<unsigned char>(0));
        return out;
    }

    BigInteger BigInteger::fromBytesBE(const std::vector<unsigned char> &bytes) {
        if (bytes.empty()) return {0};
        mpz_class out;
        const int order = 1;
        const std::size_t size = 1;
        const int endian = 1;
        const std::size_t nails = 0;
        ::mpz_import(out.get_mpz_t(), bytes.size(), order, size, endian, nails, bytes.data());
        return BigInteger(std::move(out));
    }

    BigInteger BigInteger::fromBytesLE(const std::vector<unsigned char> &bytes) {
        if (bytes.empty()) return {0};
        mpz_class out;
        const int order = -1;
        const std::size_t size = 1;
        const int endian = 1;
        const std::size_t nails = 0;
        ::mpz_import(out.get_mpz_t(), bytes.size(), order, size, endian, nails, bytes.data());
        return BigInteger(std::move(out));
    }

    BigInteger BigInteger::random_bits(std::size_t bits, std::uint64_t seed) {
        if (bits == 0) return {0};
        gmp_randclass rng(gmp_randinit_default);
        rng.seed(mpz_class(std::to_string(seed)));
        mpz_class r = rng.get_z_bits(static_cast<mp_bitcnt_t>(bits));
        return BigInteger(std::move(r));
    }

    BigInteger BigInteger::random_range(const BigInteger &upperExclusive, std::uint64_t seed) {
        requireNonNegative(upperExclusive, "random_range");
        if (upperExclusive.value_ <= 0) throw std::invalid_argument("BigInteger: upperExclusive must be > 0");
        gmp_randclass rng(gmp_randinit_default);
        rng.seed(mpz_class(std::to_string(seed)));
        mpz_class r = rng.get_z_range(upperExclusive.value_);
        return BigInteger(std::move(r));
    }

    void BigInteger::swap(BigInteger &other) noexcept {
        value_.swap(other.value_);
    }

    std::size_t BigInteger::hash() const noexcept {
        std::string s = value_.get_str();
        auto h = static_cast<std::size_t>(1469598103934665603ull);
        for (unsigned char c: s) {
            h ^= static_cast<std::size_t>(c);
            h *= static_cast<std::size_t>(1099511628211ull);
        }
        return h;
    }

    std::ostream &operator<<(std::ostream &os, const BigInteger &num) {
        os << num.value_.get_str();
        return os;
    }

    std::istream &operator>>(std::istream &is, BigInteger &num) {
        std::string s;
        is >> s;
        num = BigInteger(trim_copy(s));
        return is;
    }
} // namespace usub::mp
