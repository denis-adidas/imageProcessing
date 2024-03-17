#include <iostream>
#include "bmpAnalysis/bmpAnalysis.hpp"
#include "bmp/bmp.hpp"



int main() {

    auto test = bmp(R"(E:\Download\bmp-main\img\kodim232.bmp)");
    // auto test2 = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\restoreImage.bmp)");
    // auto test3 = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\decimationEven.bmp)");

    test.print_header();
    // test2.print_header();
    // test3.print_header();

    // test2.convertYCbCrtoRGB(test2.convertRGBtoYCbCr());

    decimateImageAvg(test);

    // auto temp = autoCorel(test, 'r', 100, 0);
    // std::cout << temp << std::endl;

    // std::cout << PSNR(test, test2, 'r') << std::endl;

    return 0;
}
