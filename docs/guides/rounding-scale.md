# Rounding & scale (BigDecimal)

`BigDecimal` keeps a fixed scale and rounds when you:

- parse from string with more fractional digits than scale
- rescale to smaller scale
- multiply/divide (keeps scale, rounds by default to HalfEven)
- compute `sqrt()` or `pow()` with negative exponent

## Practical rules

- Keep the same scale across all values you want to add/subtract.
- Use `rescaled()` to align scales:

```cpp
BigDecimal a("1.23", 2);
BigDecimal b("4.5678", 4);

auto b2 = b.rescaled(2);      // 4.57 (HalfEven)
auto sum = a + b2;            // ok
```

## HalfEven vs HalfUp

- `HalfUp`: ties always go away from zero
- `HalfEven`: ties go to the nearest even last digit (banker’s rounding)
