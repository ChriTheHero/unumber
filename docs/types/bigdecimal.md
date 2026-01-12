# BigDecimal

Fixed-scale decimal stored as an unscaled integer `mpz_class` plus `scale`.

Header: `unumber/decimal/BigDecimal.h`

## What “scale” means

If `scale = 4`, values are stored as integer *10^4*:

- `"12.3456"` => unscaled `123456`, scale `4`
- `toString()` always prints exactly 4 fractional digits.

## Construction

```cpp
explicit BigDecimal(int scale = 4);
BigDecimal(const std::string& s, int scale = 4);
BigDecimal(const char* s, int scale = 4);
BigDecimal(int v, int scale = 4);
BigDecimal(long long v, int scale = 4);
BigDecimal(double v, int scale = 4);
```

## Rounding modes

```cpp
enum class Rounding : unsigned char {
  Trunc, Floor, Ceil, HalfUp, HalfEven
};
```

Used by rescaling/div/mul/sqrt/etc.

## Scale control

```cpp
int getScale() const noexcept;

BigDecimal rescaled(int newScale, Rounding mode = Rounding::HalfEven) const;
void setScale(int newScale, Rounding mode = Rounding::HalfEven);

BigDecimal stripTrailingZeros() const;     // reduces scale when divisible by 10
BigDecimal normalize() const;              // alias
```

## Formatting

```cpp
std::string toString() const;              // current scale
std::string toString(int outScale, Rounding mode = Rounding::HalfEven) const;
double toDouble() const;
```

## Arithmetic

Important: `add/sub/mul/div` require **same scale**; mismatch throws `std::invalid_argument`.

```cpp
BigDecimal add(const BigDecimal& other) const;
BigDecimal sub(const BigDecimal& other) const;

BigDecimal mul(const BigDecimal& other, Rounding mode = Rounding::HalfEven) const;
BigDecimal div(const BigDecimal& other, Rounding mode = Rounding::HalfEven) const;
```

Operators `+ - * /` delegate to these.

## Math

```cpp
BigDecimal floor() const;
BigDecimal ceil() const;
BigDecimal round(int ndigits, Rounding mode = Rounding::HalfEven) const;
BigDecimal trunc(int ndigits) const;       // round(..., Trunc)

BigDecimal pow(int exp, Rounding mode = Rounding::HalfEven) const;
static BigDecimal sqrt(const BigDecimal& x, int outScale, Rounding mode = Rounding::HalfEven);
```

Notes:

- `pow(exp < 0)` computes reciprocal (with rounding).
- `sqrt(x < 0)` throws `std::domain_error`.

## Interop

```cpp
mpq_class toRational() const;
const mpz_class& unscaled() const noexcept;
```

## Example

```cpp
using usub::mp::BigDecimal;

BigDecimal a("12.3456", 4);
BigDecimal b("0.0044", 4);

auto c = a + b;                 // 12.3500
auto d = c.rescaled(2);         // 12.35 (HalfEven by default)
```
