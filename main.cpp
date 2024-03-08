#include <iostream>
#include "bmpAnalysis/bmpAnalysis.hpp"
#include "bmp/bmp.hpp"



int main() {

    auto test = bmp(R"(E:\Download\bmp-main\img\kodim232.bmp)");
    test.print_header();

    test.saveFileByComponent('r');
    test.rotate(1);
    auto temp = test.convertRGBtoYCbCr();
    test.convertYCbCrtoRGB(temp);



    return 0;
}
