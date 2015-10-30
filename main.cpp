#include <iostream>
#include <string>

#include "Evaluator.h"

void test()
{
    Evaluator eval("4--- -----3");
    std::cout << eval.eval() << std::endl;
}

int main()
{
    test();
    return 0;
}
