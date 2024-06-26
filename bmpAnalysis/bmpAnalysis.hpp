#ifndef BMPANALYSIS_HPP
#define BMPANALYSIS_HPP
#include <cstdint>
#include <vector>
#include <math.h>
#include <iostream>
#include <map>
#include "../bmp/bmp.hpp"

double expectedValue(bmp& image, char mod);
double sigma(double expVal, bmp& image, char mod);
double countCorel(bmp& image, char mod, char mod2);
double autoCorel(bmp& image, char mod, size_t x, size_t y);
double PSNR(bmp& srcImage, bmp& destImage, char mod);
void decimateImageEven(bmp& image, int num);
void decimateImageAvg(bmp& image);
void restoreImage(bmp& image, int num);
std::map<int, double> countProbability(bmp& image, char mod);
double calculateEntropy(bmp& image, char mod);
std::vector<uint8_t> DPCM(bmp &image, char mod, char rule);
int getComponent(char mod);
#endif
