#include <iostream>
#include "bmpAnalysis/bmpAnalysis.hpp"
#include "bmp/bmp.hpp"



int main() {

    auto test = bmp(R"(E:\Download\bmp-main\img\kodim232.bmp)");
    auto test2 = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\restoreImage.bmp)");
    test.print_header();
    test2.print_header();

    // test2.convertYCbCrtoRGB(test2.convertRGBtoYCbCr());
    // auto temp = autoCorel(test, 'r', 5, 5);
    std::cout << PSNR(test, test2, 'r') << std::endl;



    return 0;
}
