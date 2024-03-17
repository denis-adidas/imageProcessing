#ifndef BMPANALYSIS_HPP
#define BMPANALYSIS_HPP
#include <cstdint>
#include <vector>
#include <math.h>
#include <iostream>
#include "../bmp/bmp.hpp"

double expectedValue(bmp& image, char mod);
double sigma(double expVal, bmp& image, char mod);
double countCorel(bmp& image, char mod, char mod2);
double autoCorel(bmp& image, char mod, size_t x, size_t y);
double PSNR(bmp& srcImage, bmp& destImage, char mod);
std::vector<uint8_t> decimationEven(bmp& image);
void decimateImageEven(bmp& image, int num);
void decimateImageAvg(bmp& image);

#endif
