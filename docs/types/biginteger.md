# BigInteger

Arbitrary-precision **signed** integer built on `mpz_class`.

Header: `unumber/integer/BigInteger.h`

## Construction

```cpp
BigInteger();                              // 0
BigInteger("123456");                      // base 10
BigInteger("FF", 16);                      // base 16 (2..62 or 0 for GMP auto)
BigInteger(123);
BigInteger(long long v);
BigInteger(unsigned long long v);
BigInteger(const mpz_class& v);
```

Also:

```cpp
static BigInteger fromString(const std::string& s, int base);
static BigInteger fromBytesBE(const std::vector<unsigned char>& bytes);
static BigInteger fromBytesLE(const std::vector<unsigned char>& bytes);
```

## Formatting

```cpp
std::string toString() const;              // base 10
std::string toString(int base) const;      // base 2..62 (or 0 for GMP auto)
```

## Arithmetic

`+ - * / %` and compound ops `+= -= *= /= %=`.

Division by zero throws `std::domain_error`.

## Bit operations

`& | ^ ~ << >>` with compound forms.

Bit ops require non-negative in:

- `bit_length()`
- `popcount()`
- `bit_test()`
- `with_bit_set()`
- `with_bit_clear()`
- `bit_set()`
- `bit_clear()`
- `toBytesBE()/toBytesLE()`
- random helpers with range upper bound

## Number theory helpers

```cpp
static std::pair<BigInteger, BigInteger> divmod(const BigInteger& a, const BigInteger& b);
static BigInteger gcd(const BigInteger& a, const BigInteger& b);
static BigInteger lcm(const BigInteger& a, const BigInteger& b);

BigInteger pow(std::uint64_t exp) const;
BigInteger pow_mod(const BigInteger& exp, const BigInteger& mod) const;
BigInteger pow_mod(std::uint64_t exp, const BigInteger& mod) const;

BigInteger mod(const BigInteger& modulus) const;
BigInteger inv_mod(const BigInteger& modulus) const;

bool is_probable_prime(int reps = 25) const;
BigInteger next_prime() const;
```

Rules:

- `inv_mod()` throws `std::domain_error` if inverse does not exist.
- `mod()` and `pow_mod()` normalize negative modulus to `abs(mod)`.

## Conversions

```cpp
long long toInt64Exact() const;
unsigned long long toUint64Exact() const;
```

Throws `std::out_of_range` if out of range, and for unsigned also if negative.

## Random

```cpp
static BigInteger random_bits(std::size_t bits, std::uint64_t seed);
static BigInteger random_range(const BigInteger& upperExclusive, std::uint64_t seed);
```

`random_range()` requires `upperExclusive > 0` and non-negative.

## Example

```cpp
using usub::mp::BigInteger;

BigInteger n("12345678901234567890");
auto p = n.next_prime();
auto inv = BigInteger(3).inv_mod(BigInteger(11)); // 4
```
