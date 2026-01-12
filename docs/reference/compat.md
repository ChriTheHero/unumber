# Compatibility notes

- String parsing relies on GMP behavior for `mpz_class`, `mpq_class`, `mpf_set_str`.
- `BigDecimal` parsing is implemented manually and accepts `_` separators plus optional exponent.
- `BigFloat::toString()` chooses digits derived from precision; use fixed/scientific for explicit control.
