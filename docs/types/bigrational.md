# BigRational

Exact rational number (`mpq_class`).

Header: `unumber/rational/BigRational.h`

## Construction

```cpp
BigRational();                       // 0
BigRational(const std::string& s);   // e.g. "123/456" or "3.14" (GMP parsing rules)
BigRational(int v);
BigRational(long long v);
BigRational(const mpq_class& v);
BigRational(int num, int den);
BigRational(const BigInteger& num);
BigRational(const BigFloat& num);    // via BigFloat::toString()
```

## Operators

`+ - * /` and compounds, comparisons.

Stream:

```cpp
std::ostream& operator<<(std::ostream&, const BigRational&);
std::istream& operator>>(std::istream&, BigRational&);
```

## Formatting

```cpp
std::string toString() const;        // mpq_class::get_str()
```

## Example

```cpp
using usub::mp::BigRational;

BigRational a(1, 3);
BigRational b("2/3");
std::cout << (a + b).toString() << "\n"; // "1"
```
