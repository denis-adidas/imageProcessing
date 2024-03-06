#include <iostream>
#include "bmpAnalysis/bmpAnalysis.hpp"
#include "bmp/bmp.hpp"



int main() {

    auto test = bmp(R"(E:\Download\bmp-main\img\kodim232.bmp)");
    test.print_header();

    test.saveFileByComponent('r');

    // auto test2 = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\kodim232.bmp)");
    // auto temp = test.saveFileByComponent('r');
    test.rotate(1);
    test.saveImage("../data/rotateImg.bmp", test.getData());

    // auto test2 = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\rotateImg.bmp)");
    // test2.print_header();


    // test2.print_header();

    // test.saveFileByComponent('r');
    // temp = test.convertRGBtoYCbCr();

    // test.saveFileByComponentYCbCr('R', temp);

    // std::cout << expectedValue(test, 'r') << std::endl;
    // std::cout << sigma(73.5, test, 'r') << std::endl;

    return 0;
}
