#include <iostream>
#include <string>

#include "Evaluator.h"

std::string argv_concatenate(int argc, const char*& argv[])
{
    std::string to_return;
    for (--argc; argc > 0; --argc)
        to_return += argv[argc];

    return to_return;
}

int main(int argc, char* argv[])
{


    std::cout << "Hello world!\n";
    return 0;
}
