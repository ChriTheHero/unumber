#include <gmp.h>
#include "unumber/decimal/BigDecimal.h"
#include "unumber/integer/BigInteger.h"
#include "unumber/float/BigFloat.h"
#include "unumber/rational/BigRational.h"
#include "unumber/matrix/BigMatrix.h"
#include "unumber/modular/BigModular.h"

using namespace usub::mp;

int main() { {
        BigDecimal a = "1234567890123.4567";
        BigDecimal b = "987654321098.7654";

        BigDecimal sum = a + b;
        BigDecimal product = a * b;
        BigDecimal quotient = a / b;

        std::cout << "Amount: " << sum << std::endl;
        std::cout << "Product: " << product << std::endl;
        std::cout << "Quotient: " << quotient << std::endl;
    } {
        BigInteger a("123456789012345678901234567890");
        BigInteger b("987654321098765432109876543210");

        BigInteger sum = a + b;
        BigInteger diff = a - b;
        BigInteger prod = a * b;
        BigInteger quot = b / a;
        BigInteger mod = b % a;

        std::cout << "a = " << a << std::endl;
        std::cout << "b = " << b << std::endl;
        std::cout << "a + b = " << sum << std::endl;
        std::cout << "a - b = " << diff << std::endl;
        std::cout << "a * b = " << prod << std::endl;
        std::cout << "b / a = " << quot << std::endl;
        std::cout << "b % a = " << mod << std::endl;
    } {
        // BigRational
        BigRational r1(3, 4); // 3/4
        BigRational r2(2, 5); // 2/5
        BigRational r3 = r1 + r2;
        std::cout << "BigRational Sum: " << r3 << std::endl;

        // BigFloat
        BigFloat f1("3.141592653589793");
        BigFloat f2("2.718281828459045");
        BigFloat f3 = f1 * f2;
        std::cout << "BigFloat Product: " << f3 << std::endl;

        // BigModular
        BigInteger mod("1000000007");
        BigModular m1(BigInteger("123456789"), mod);
        BigModular m2(BigInteger("987654321"), mod);
        BigModular m3 = m1 * m2;
        std::cout << "BigModular Product: " << m3 << std::endl;

        // BigMatrix<BigInteger>
        BigMatrix<BigInteger> mat1(2, 2), mat2(2, 2);
        mat1[0][0] = BigInteger("1");
        mat1[0][1] = BigInteger("2");
        mat1[1][0] = BigInteger("3");
        mat1[1][1] = BigInteger("4");

        mat2[0][0] = BigInteger("5");
        mat2[0][1] = BigInteger("6");
        mat2[1][0] = BigInteger("7");
        mat2[1][1] = BigInteger("8");

        BigMatrix<BigInteger> mat3 = mat1 * mat2;
        std::cout << "BigMatrix<BigInteger> Product: \n" << mat3 << std::endl;

        // BigMatrix<BigRational>
        BigMatrix<BigRational> matR1(2, 2), matR2(2, 2);
        matR1[0][0] = BigRational(1, 2);
        matR1[0][1] = BigRational(2, 3);
        matR1[1][0] = BigRational(3, 4);
        matR1[1][1] = BigRational(4, 5);

        matR2[0][0] = BigRational(5, 6);
        matR2[0][1] = BigRational(6, 7);
        matR2[1][0] = BigRational(7, 8);
        matR2[1][1] = BigRational(8, 9);

        BigMatrix<BigRational> matR3 = matR1 * matR2;
        std::cout << "BigMatrix<BigRational> Product: \n" << matR3 << std::endl;

        // BigMatrix<BigFloat>
        BigMatrix<BigFloat> matF1(2, 2), matF2(2, 2);
        matF1[0][0] = BigFloat("1.1");
        matF1[0][1] = BigFloat("2.2");
        matF1[1][0] = BigFloat("3.3");
        matF1[1][1] = BigFloat("4.4");

        matF2[0][0] = BigFloat("5.5");
        matF2[0][1] = BigFloat("6.6");
        matF2[1][0] = BigFloat("7.7");
        matF2[1][1] = BigFloat("8.8");

        BigMatrix<BigFloat> matF3 = matF1 * matF2;
        std::cout << "BigMatrix<BigFloat> Product: \n" << matF3 << std::endl;
    }

    return 0;
}
