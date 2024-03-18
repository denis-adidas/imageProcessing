#include <iostream>
#include "bmpAnalysis/bmpAnalysis.hpp"
#include "bmp/bmp.hpp"



int main() {

    auto test = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)");
    // auto test2 = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\restoreImage.bmp)");
     auto test3 = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\restoreImage.bmp)");

    test.print_header();
    // test2.print_header();
     test3.print_header();

    // std::cout << calculateEntropy(test, 'r') << std::endl;
    // test2.convertYCbCrtoRGB(test2.convertRGBtoYCbCr());

//    decimateImageAvg(test);
    // restoreImage(test3, 2);
    std::cout << DPCM(test, 'r', 4).size() << std::endl;
    // auto temp = autoCorel(test, 'r', 50, 0);

    // std::cout << temp << std::endl;

    // std::cout << PSNR(test, test2, 'r') << std::endl;

    return 0;
}
