# Quickstart

```cpp
#include <iostream>
#include "unumber/integer/BigInteger.h"
#include "unumber/decimal/BigDecimal.h"
#include "unumber/float/BigFloat.h"
#include "unumber/rational/BigRational.h"
#include "unumber/modular/BigModular.h"

int main() {
    using namespace usub::mp;

    BigInteger a("123456789012345678901234567890");
    BigInteger b(42);

    std::cout << (a + b) << "\n";
    std::cout << a.pow(3).toString() << "\n";

    BigDecimal d1("12.3456", 4);
    BigDecimal d2("0.0044", 4);
    std::cout << (d1 + d2).toString() << "\n";     // fixed scale

    BigFloat f("3.1415926535", 256);
    std::cout << f.toStringScientific() << "\n";

    BigRational q(1, 3);
    std::cout << (q + BigRational("2/3")).toString() << "\n";

    BigModular x(BigInteger(10), BigInteger(17));
    BigModular y(BigInteger(3), BigInteger(17));
    std::cout << (x / y).toString() << "\n";       // uses inverse()
}
```
