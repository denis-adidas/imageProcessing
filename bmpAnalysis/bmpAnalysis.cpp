#include "bmpAnalysis.hpp"


double expectedValue(bmp& image, char mod) {
    double M;
    const size_t bytesPerPixel = image.getInfoHeader().bi_bit_count / 8;
    auto H = image.getInfoHeader().bi_height;
    auto W = image.getInfoHeader().bi_width;

    double sum = 0;

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            size_t pixelOffset = (i * image.getInfoHeader().bi_width + j) * bytesPerPixel;


            uint8_t componentValue;
            switch (mod) {
                case 'r':
                    componentValue = image.getData()[pixelOffset];
                break;
                case 'g':
                    componentValue = image.getData()[pixelOffset + 1];
                break;
                case 'b':
                    componentValue = image.getData()[pixelOffset + 2];
                break;
                default:
                        return 0.0;
            }

            sum += static_cast<double>(componentValue);
        }
    }
    M = (1.0 / (W * H)) * sum;
    return M;
}
double sigma(double expVal, bmp& image, char mod) {
    double sigma;

    const size_t bytesPerPixel = image.getInfoHeader().bi_bit_count / 8;
    auto H = image.getInfoHeader().bi_height;
    auto W = image.getInfoHeader().bi_width;

    double sum = 0;

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            size_t pixelOffset = (i * image.getInfoHeader().bi_width + j) * bytesPerPixel;


            uint8_t componentValue;
            switch (mod) {
                case 'r':
                    componentValue = image.getData()[pixelOffset] - expVal;
                break;
                case 'g':
                    componentValue = image.getData()[pixelOffset + 1] - expVal;
                break;
                case 'b':
                    componentValue = image.getData()[pixelOffset + 2] - expVal;
                break;
                default:
                    return 0.0;
            }
            sum += static_cast<double>(componentValue);
        }
    }
    sigma = std::sqrt((1.0 / ((W * H) - 1)) * std::pow(sum, 2));
    return sigma;
}
double countCorel(double expValA, double expValB, double sigmaA, double sigmaB, bmp& image, char mod, char mod2) {
    double r;
    double mul;
    double M;

    const size_t bytesPerPixel = image.getInfoHeader().bi_bit_count / 8;
    auto H = image.getInfoHeader().bi_height;
    auto W = image.getInfoHeader().bi_width;

    double sumA = 0;
    double sumB = 0;

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            size_t pixelOffset = (i * image.getInfoHeader().bi_width + j) * bytesPerPixel;


            uint8_t componentValue;
            switch (mod) {
                case 'r':
                    componentValue = image.getData()[pixelOffset] - expValA;
                break;
                case 'g':
                    componentValue = image.getData()[pixelOffset + 1] - expValA;
                break;
                case 'b':
                    componentValue = image.getData()[pixelOffset + 2] - expValA;
                break;
                default:
                    return 0.0;
            }
            sumA += static_cast<double>(componentValue);
            switch (mod2) {
                case 'r':
                    componentValue = image.getData()[pixelOffset] - expValB;
                break;
                case 'g':
                    componentValue = image.getData()[pixelOffset + 1] - expValB;
                break;
                case 'b':
                    componentValue = image.getData()[pixelOffset + 2] - expValB;
                break;
                default:
                    return 0.0;
            }
            sumB += static_cast<double>(componentValue);

            mul = sumA*sumB;
        }
    }
    M = (1.0 / (W * H)) * mul;
    r = M / (sigmaA * sigmaB);
    return r;
}