#include <iostream>
#include "bmpAnalysis/bmpAnalysis.hpp"
#include "bmp/bmp.hpp"



int main() {

    auto test = bmp(R"(E:\Download\bmp-main\img\kodim232.bmp)");
    test.print_header();

    auto temp = autoCorel(test, 'r', 'g', 5, 5);
    std::cout << temp << std::endl;





    return 0;
}
