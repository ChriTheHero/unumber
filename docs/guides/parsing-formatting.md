# Parsing & formatting

## BigInteger

- Default parsing is base 10.
- `BigInteger(str, base)` supports base `2..62` (and `0` for GMP auto).
- `toString(base)` uses the same base rules.

## BigDecimal

- Accepts:
  - optional sign
  - fractional part `.` 
  - exponent `e` / `E`
  - underscores `_` as separators (removed before parsing)
- Parsing rounds to the instance scale using `HalfEven`.

Examples:

```cpp
BigDecimal a("1.234567", 4);      // rounds to 1.2346 (HalfEven)
BigDecimal b("1.23445", 4);       // ties to even -> 1.2344
BigDecimal c("12e-3", 6);         // 0.012000
```

## BigFloat

- Uses GMP `mpf_set_str(..., base=10)`.
- Output:
  - `toStringScientific()` for stable representation
  - `toStringFixed(digits)` for fixed digits
