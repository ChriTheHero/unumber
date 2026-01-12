# Interop with GMP

## Raw access

`BigInteger` exposes:

```cpp
const mpz_class& raw() const noexcept;
mpz_class& raw() noexcept;
```

Use it when you need direct GMP calls.

`BigDecimal` exposes:

```cpp
const mpz_class& unscaled() const noexcept;
mpq_class toRational() const;
```

`BigFloat` is internally an `mpf_class`. It does not expose `raw()` in the current header, but you can convert to string or use `mpf_class` constructor if you already have one.

`BigRational` wraps `mpq_class` internally and formats through `get_str()`.

## Thread safety

GMP objects are not inherently thread-safe when the *same object* is mutated concurrently. Treat these types as regular C++ value types: don't race on shared instances.
