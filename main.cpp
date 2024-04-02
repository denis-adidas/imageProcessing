#include <iostream>
#include "bmpAnalysis/bmpAnalysis.hpp"
#include "bmp/bmp.hpp"



int main() {

    auto test = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)");


    auto test2 = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\YCbCr_component.bmp)");

    test.print_header();
    test2.print_header();


    // auto tmp = test.convertRGBtoYCbCr();
    // test.convertYCbCrtoRGB(tmp);

    // decimateImageEven(test, 2);

    // auto test3 = bmp(R"(D:\CLion_projects\crypto\imageAnalysis1\data\decimationEven.bmp)");
    // test3.print_header();
    // restoreImage(test3, 2);

    // test3.saveFileByComponent('r');
    // test3.saveFileByComponent('g');
    // test3.saveFileByComponent('b');

    // std::cout << calculateEntropy(test, 'r') << std::endl;
    // test2.convertYCbCrtoRGB(test2.convertRGBtoYCbCr());


    //    decimateImageAvg(test);
//     restoreImage(test2, 2);
    // std::cout << DPCM(test, 'r', 4).size() << std::endl;

//    std::cout << "Entropy r: " << calculateEntropy(test, 'r') << std::endl;
//    std::cout << "Entropy g: " << calculateEntropy(test, 'g') << std::endl;
//    std::cout << "Entropy b: " << calculateEntropy(test, 'b') << std::endl;

    std::cout << "Entropy y: " << calculateEntropy(test2, 'r') << std::endl;
    std::cout << "Entropy cb: " << calculateEntropy(test2, 'g') << std::endl;
    std::cout << "Entropy cr: " << calculateEntropy(test2, 'b') << std::endl;

    // std::cout << "RGB to YCbCr to RGB: R component: " << PSNR(test, test3, 'r') << std::endl;
    // std::cout << "RGB to YCbCr to RGB: G component: " << PSNR(test, test3, 'g') << std::endl;
    // std::cout << "RGB to YCbCr to RGB: B component: " << PSNR(test, test3, 'b') << std::endl;

    return 0;
}
