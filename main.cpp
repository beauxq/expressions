#include <iostream>
#include <string>

#include "Evaluator.h"

void test()
{
    Evaluator eval("4---- -----3");  // valid
    //Evaluator eval("4 = 3");  // invalid operator
    //Evaluator eval("4 ! 3");  // unary not allowed !
    //Evaluator eval("15+3 2");  // binary required  *
    //Evaluator eval("4 + + 3");  // binary not allowed +
    //Evaluator eval("<3+2");  // start  *
    //Evaluator eval("3&&&& 5");  // start group  *
    //Evaluator eval("4*");  // end
    // TODO test unmatched parentheses
    // TODO test 0^0 (3-3)^(8-8)
    // TODO test negative exponent 4^-7 (other exponents -1,0,1)
    std::cout << eval.eval() << std::endl;

    // TODO test spaces at begin and end
    std::cout << eval.eval("1+2*3") << std::endl;
    std::cout << eval.eval("2+2^2*3") << std::endl;
    std::cout << eval.eval("1==2") << std::endl;
    std::cout << eval.eval("1+3 > 2") << std::endl;
    std::cout << eval.eval("(4>=4) && 0") << std::endl;
    std::cout << eval.eval("(1+2)*3") << std::endl;
    std::cout << eval.eval("++++2-5*(3^2)") << std::endl;
}

int main()
{
    test();
    return 0;
}
