#ifndef BMPANALYSIS_HPP
#define BMPANALYSIS_HPP
#include <cstdint>
#include <vector>
#include <math.h>
#include "../bmp/bmp.hpp"

double expectedValue(bmp& image, char mod);
double sigma(double expVal, bmp& image, char mod);
double countCorel(bmp& image, char mod, char mod2);
double autoCorel(bmp& image, char mod, char mod2, size_t x, size_t y);
double autoCorel2(bmp& image, char mod, char mod2, size_t x, size_t y);



#endif
