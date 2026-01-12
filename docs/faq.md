# FAQ

## Why GMP?

It clearly beats any hand-rolled bigint for correctness and performance, and it already exists everywhere.

## Why BigDecimal is fixed-scale?

So you get deterministic output and predictable rounding.

## Can I mix BigDecimal scales?

Not directly for arithmetic. Rescale first.

## Is BigMatrix optimized?

No. It is a convenience wrapper.
