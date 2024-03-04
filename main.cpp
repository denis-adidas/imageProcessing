#include <iostream>
#include "bmpAnalysis/bmpAnalysis.hpp"
#include "bmp/bmp.hpp"



int main() {

    auto test = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\kodim232.bmp)");
    auto temp = test.saveFileByComponent('r');

    // std::cout << expectedValue(test, 'r') << std::endl;
    std::cout << sigma(73.5, test, 'r') << std::endl;

    return 0;
}
