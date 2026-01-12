# unumber

`unumber` is a small C++23 library of arbitrary-precision number types built on **GMP** (`gmp` / `gmpxx`).

Included types:

- **BigInteger** — arbitrary-size signed integer (`mpz_class`)
- **BigDecimal** — fixed-scale decimal with configurable rounding (stored as `mpz_class` + `scale`)
- **BigFloat** — arbitrary-precision float (`mpf_class`) with explicit precision control
- **BigRational** — exact rational (`mpq_class`)
- **BigModular** — integers modulo *m* (with inversion via `mpz_invert`)
- **BigMatrix<T>** — simple matrix for `BigInteger | BigDecimal | BigFloat | BigRational`

## Status

This documentation describes the public API exposed by the headers in `include/unumber/**`.

## Requirements

- C++23 compiler
- GMP development packages (both `gmp` and `gmpxx`)
