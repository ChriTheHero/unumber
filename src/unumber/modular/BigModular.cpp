#include "unumber/modular/BigModular.h"

#include <ostream>
#include <stdexcept>

namespace usub::mp {
    void BigModular::requireValidModulus(const mpz_class &m) {
        if (m == 0) {
            throw std::invalid_argument("BigModular: modulus is zero");
        }
    }

    void BigModular::requireSameModulus(const BigModular &a, const BigModular &b) {
        if (a.modulus_ != b.modulus_) {
            throw std::invalid_argument("BigModular: modulus mismatch");
        }
    }

    mpz_class BigModular::norm(const mpz_class &v, const mpz_class &m) {
        mpz_class mm = m;
        if (mm < 0) mm = -mm;
        mpz_class r;
        ::mpz_mod(r.get_mpz_t(), v.get_mpz_t(), mm.get_mpz_t());
        return r;
    }

    BigModular::BigModular() = default;

    BigModular::BigModular(const mpz_class &v, const mpz_class &m)
        : value_(norm(v, m)), modulus_(m) {
        requireValidModulus(modulus_);
        if (modulus_ < 0) modulus_ = -modulus_;
    }

    BigModular::BigModular(const BigInteger &val, const BigInteger &mod)
        : value_(0), modulus_(mod.raw()) {
        requireValidModulus(modulus_);
        if (modulus_ < 0) modulus_ = -modulus_;
        value_ = norm(val.raw(), modulus_);
    }

    BigModular BigModular::operator+(const BigModular &other) const {
        requireSameModulus(*this, other);
        return BigModular(value_ + other.value_, modulus_);
    }

    BigModular BigModular::operator-(const BigModular &other) const {
        requireSameModulus(*this, other);
        return BigModular(value_ - other.value_, modulus_);
    }

    BigModular BigModular::operator*(const BigModular &other) const {
        requireSameModulus(*this, other);
        return BigModular(value_ * other.value_, modulus_);
    }

    BigModular BigModular::operator/(const BigModular &other) const {
        requireSameModulus(*this, other);
        return (*this) * other.inverse();
    }

    BigModular BigModular::inverse() const {
        requireValidModulus(modulus_);
        mpz_class inv;
        const int ok = ::mpz_invert(inv.get_mpz_t(), value_.get_mpz_t(), modulus_.get_mpz_t());
        if (ok == 0) {
            throw std::runtime_error("BigModular: inverse does not exist");
        }
        return BigModular(inv, modulus_);
    }

    BigModular &BigModular::operator+=(const BigModular &other) {
        requireSameModulus(*this, other);
        value_ = norm(value_ + other.value_, modulus_);
        return *this;
    }

    BigModular &BigModular::operator-=(const BigModular &other) {
        requireSameModulus(*this, other);
        value_ = norm(value_ - other.value_, modulus_);
        return *this;
    }

    BigModular &BigModular::operator*=(const BigModular &other) {
        requireSameModulus(*this, other);
        value_ = norm(value_ * other.value_, modulus_);
        return *this;
    }

    BigModular &BigModular::operator/=(const BigModular &other) {
        requireSameModulus(*this, other);
        return (*this) *= other.inverse();
    }

    bool BigModular::operator==(const BigModular &other) const {
        return value_ == other.value_ && modulus_ == other.modulus_;
    }

    bool BigModular::operator!=(const BigModular &other) const {
        return !(*this == other);
    }

    std::ostream &operator<<(std::ostream &os, const BigModular &num) {
        os << num.value_.get_str();
        return os;
    }

    std::string BigModular::toString() const {
        return value_.get_str();
    }
} // namespace usub::mp
