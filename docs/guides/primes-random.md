# Primes & random (BigInteger)

## Probable primes

```cpp
BigInteger n("123456789012345678901234567890");
bool ok = n.is_probable_prime(25);
auto p = n.next_prime();
```

`is_probable_prime()` delegates to GMP's probabilistic test.

## Random

```cpp
auto x = BigInteger::random_bits(256, 12345);
auto y = BigInteger::random_range(BigInteger("1000000000000"), 12345);
```

Notes:

- Deterministic given the same seed.
- `random_range()` requires `upperExclusive > 0`.
