#include <iostream>
#include <string>

#include "Evaluator.h"

void test()
{
    Evaluator eval("4---- -----5");

    std::cout << eval.eval() << std::endl;

    // tests for errors
    //std::cout << eval.eval("4 = 3");  // invalid operator
    //std::cout << eval.eval("4 ! 3");  // unary not allowed !
    //std::cout << eval.eval("15+3 2");  // binary required  *
    //std::cout << eval.eval("4 + + 3");  // binary not allowed +
    //std::cout << eval.eval("<3+2");  // start  *
    //std::cout << eval.eval("3&&&& 5");  // start group  *
    //std::cout << eval.eval("4*");  // end
    //std::cout << eval.eval("(4 + 3)) * 2");  // unmatched parentheses
    //std::cout << eval.eval("(3-3)^(8-8)");  // 0^0
    //std::cout << eval.eval("2 / (8-8)");  // divide by zero
    //std::cout << eval.eval("4^-7");  // negative exponent (not base -1,0,1)

    // 7 example valid expressions from specifications
    std::cout << eval.eval("1+2*3") << std::endl;
    std::cout << eval.eval("2+2^2*3") << std::endl;
    std::cout << eval.eval("1==2") << std::endl;
    std::cout << eval.eval("1+3 > 2") << std::endl;
    std::cout << eval.eval("(4>=4) && 0") << std::endl;
    std::cout << eval.eval("(1+2)*3") << std::endl;
    std::cout << eval.eval("++++2-5*(3^2)") << std::endl;

    // other special cases
    std::cout << eval.eval("-1^-7") << std::endl;
    std::cout << eval.eval("0^-4") << std::endl;
    std::cout << eval.eval("   2   + 2") << std::endl;
    std::cout << eval.eval("2+2          ") << std::endl;
    std::cout << eval.eval("5*2+2^2*++!3/2- --8%3") << std::endl;
    std::cout << eval.eval("5*(2+2^(2*++!3/2)- --8)%3") << std::endl;

    // examples from report
    std::cout << eval.eval("--- -----2") << std::endl;
    std::cout << eval.eval("--------2") << std::endl;
    std::cout << eval.eval("4--3") << std::endl;
    std::cout << eval.eval("4---3") << std::endl;
    std::cout << eval.eval("4----3") << std::endl;
    std::cout << eval.eval("4---- ---3") << std::endl;

    // example errors from report
    //std::cout << eval.eval("9*+++5") << std::endl;
    //std::cout << eval.eval("9*6++5") << std::endl;
    //std::cout << eval.eval("15+3 2") << std::endl;
}

void ui()
{
    Evaluator eval;
    std::string input;

    std::cout << "\ntype an expression, \"exit\" to end\n";

    while (true)
    {
        std::cout << ">>> ";
        std::getline(std::cin, input);

        if (input[0] == 'e' || input[0] == 'E')
            break;

        try
        {
            std::cout << eval.eval(input) << std::endl;
        }
        catch (std::invalid_argument err)
        {
            std::cout << "Error: " << err.what() << std::endl;
        }
    }
}

int main()
{
    test();

    ui();
    return 0;
}
