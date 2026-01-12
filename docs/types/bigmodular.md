# BigModular

Modular arithmetic: values in **Z/mZ**.

Header: `unumber/modular/BigModular.h`

## Invariants

- `modulus != 0`
- modulus is normalized to non-negative
- `value` is reduced into `[0, modulus)` (using `mpz_mod`)

## Construction

```cpp
BigModular(); // value=0, modulus=1
BigModular(const BigInteger& value, const BigInteger& modulus);
```

## Operators

`+ - * /` and compounds.

Division uses modular inverse:

```cpp
BigModular inverse() const;          // throws if inverse does not exist
```

Rules:

- Any operation between two modular numbers requires equal modulus, otherwise `std::invalid_argument`.

## Example

```cpp
using usub::mp::{BigInteger, BigModular};

BigModular a(BigInteger(10), BigInteger(17));
BigModular b(BigInteger(3), BigInteger(17));

auto c = a / b; // 10 * inv(3) mod 17 = 10*6 = 9
std::cout << c.toString() << "\n";
```
