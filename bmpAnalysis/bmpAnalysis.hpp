#ifndef BMPANALYSIS_HPP
#define BMPANALYSIS_HPP
#include <cstdint>
#include <vector>
#include <math.h>
#include "../bmp/bmp.hpp"

double expectedValue(bmp& image, char mod);
double sigma(double expVal, bmp& image, char mod);
double countCorel(double expValA, double expValB, double sigmaA, double sigmaB, bmp& image, char mod, char mod2);



#endif
