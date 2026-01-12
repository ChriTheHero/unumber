# BigMatrix

`BigMatrix<T>` is a small matrix wrapper over `std::vector<std::vector<T>>`.

Header: `unumber/matrix/BigMatrix.h`

## Supported types

Compile-time restricted to:

- `BigInteger`
- `BigRational`
- `BigFloat`
- `BigDecimal`

## Construction

```cpp
BigMatrix<T>(size_t rows, size_t cols);
BigMatrix<T>(const std::vector<std::vector<T>>& values);
```

## Operations

```cpp
BigMatrix operator+(const BigMatrix& other) const;
BigMatrix operator-(const BigMatrix& other) const;
BigMatrix operator*(const BigMatrix& other) const;
BigMatrix transpose() const;
```

Dimension checks throw `std::invalid_argument`.

## Access

```cpp
std::vector<T>& operator[](size_t i);
const std::vector<T>& operator[](size_t i) const;
```

## Streams

`<<` prints rows as space-separated values.

`>>` reads elements into the existing matrix shape.

## Example

```cpp
using usub::mp::BigMatrix;
using usub::mp::BigInteger;

BigMatrix<BigInteger> A({
  {BigInteger(1), BigInteger(2)},
  {BigInteger(3), BigInteger(4)},
});

auto AT = A.transpose();
std::cout << AT;
```
