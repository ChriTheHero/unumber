# Exceptions & error rules

This library uses standard exceptions.

## BigInteger

- division/mod by zero: `std::domain_error`
- invalid base: `std::invalid_argument`
- negative input for bit operations / bytes / ranges: `std::invalid_argument`
- exact conversion overflow: `std::out_of_range`

## BigDecimal

- scale mismatch (add/sub/mul/div/compare/min/max/clamp): `std::invalid_argument`
- division by zero: `std::domain_error`
- invalid numbers during parsing: `std::invalid_argument`
- sqrt of negative: `std::domain_error`

## BigFloat

- precision = 0: `std::invalid_argument`
- invalid string: `std::invalid_argument`

## BigModular

- modulus = 0: `std::invalid_argument`
- modulus mismatch in ops: `std::invalid_argument`
- inverse does not exist: `std::runtime_error`
