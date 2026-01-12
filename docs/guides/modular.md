# Modular arithmetic

## Modulus rules

- modulus cannot be zero
- modulus is normalized to `abs(modulus)`

## Inversion

`inverse()` fails when `gcd(value, modulus) != 1`.

```cpp
BigModular a(BigInteger(2), BigInteger(4));
a.inverse(); // throws (no inverse)
```

## Division

`a / b` is `a * b.inverse()`.
