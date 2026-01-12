# BigFloat

Arbitrary-precision floating point number built on `mpf_class`.

Header: `unumber/float/BigFloat.h`

## Precision model

Each instance has a precision in **bits**. Operations pick `max(lhs.precision, rhs.precision)` for the result.

```cpp
mp_bitcnt_t getPrecision() const noexcept;
void setPrecision(mp_bitcnt_t prec);
```

## Construction

```cpp
BigFloat();                                      // 0, 256 bits
BigFloat(const std::string& s, mp_bitcnt_t p=256);
BigFloat(const char* s, mp_bitcnt_t p=256);
BigFloat(double v, mp_bitcnt_t p=256);
BigFloat(const mpf_class& v);
BigFloat(const BigInteger& v, mp_bitcnt_t p=256);
```

## Formatting

```cpp
std::string toString() const;                    // fixed with digits derived from precision
std::string toStringScientific() const;          // mantissa + e10
std::string toStringFixed(int digits) const;     // fixed digits after decimal
```

## Operators

`+ - * /`, compounds, comparisons, stream `<< >>`.

Notes:

- `toString()` is intended as a reasonable default; use `toStringScientific()` for stable output across magnitudes.
- Input parsing uses base-10 strings via `mpf_set_str`.

## Example

```cpp
using usub::mp::BigFloat;

BigFloat x("1.0", 512);
BigFloat y("3.0", 512);
auto z = x / y;

std::cout << z.toStringScientific() << "\n";
std::cout << z.toStringFixed(50) << "\n";
```
