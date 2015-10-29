#include <iostream>
#include <string>

#include "Evaluator.h"

void test()
{
    Evaluator eval("238");
    std::cout << eval.eval() << std::endl;
}

int main()
{
    test();
    return 0;
}
