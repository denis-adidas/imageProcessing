#include "bmpAnalysis.hpp"

#include <valarray>


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
double countCorel(bmp& image, char mod, char mod2) {
    double r;
    double mul;
    double M;

    double expValA {}, expValB {}, sigmaA {}, sigmaB {};

    expValA = expectedValue(image, mod);
    expValB = expectedValue(image, mod2);

    sigmaA = (expValA, image, mod);
    sigmaB = (expValB, image, mod);

    const size_t bytesPerPixel = image.getInfoHeader().bi_bit_count / 8;
    auto H = image.getInfoHeader().bi_height;
    auto W = image.getInfoHeader().bi_width;

    double sumA = 0;
    double sumB = 0;

    mul = 0;

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
            sumA = static_cast<double>(componentValue);
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
            sumB = static_cast<double>(componentValue);

            mul += sumA * sumB;
        }
    }
    M = (1.0 / (W * H)) * mul;
    r = M / (sigmaA * sigmaB);
    return r;
}

double autoCorel(bmp &image, char mod, const size_t x, const size_t y) {
    double expVal1, expVal2;
    double sigma1, sigma2;
    double r;
    const size_t bytesPerPixel = image.getInfoHeader().bi_bit_count / 8;
    auto H = image.getInfoHeader().bi_height;
    auto W = image.getInfoHeader().bi_width;

    uint8_t bitShift;
    switch (mod) {
        case 'r':
            bitShift = 0;
        break;
        case 'g':
            bitShift = 1;
        break;
        case 'b':
            bitShift = 2;
        break;
        default:
            return 0.0;
    }

    double sum = 0;

    for (int i = 1; i < H - y; ++i) {
        for (int j = 1; j < W - x; ++j) {
            size_t pixelOffset = (i * image.getInfoHeader().bi_width + j) * bytesPerPixel;
            uint8_t componentValue = image.getData()[pixelOffset + bitShift];
            sum += static_cast<double>(componentValue);
        }
    }
    expVal1 = (1.0 / (W * H)) * sum;

    sum = 0.0;
    for (int i = y + 1;  i < H; ++i) {
        for (int j = x + 1; j < W - x; ++j) {
            size_t pixelOffset = (i * image.getInfoHeader().bi_width + j) * bytesPerPixel;
            uint8_t componentValue = image.getData()[pixelOffset + bitShift];
            sum += static_cast<double>(componentValue);
        }
    }
    expVal2 = (1.0 / (W * H)) * sum;

    sum = 0.0;
    for (int i = 1; i < H - y; ++i) {
        for (int j = 1; j < W - x; ++j) {
            size_t pixelOffset = (i * image.getInfoHeader().bi_width + j) * bytesPerPixel;
            uint8_t componentValue = image.getData()[pixelOffset + bitShift] - expVal1;
            sum += static_cast<double>(componentValue);
        }
    }
    sigma1 = std::sqrt((1.0 / ((W * H) - 1)) * std::pow(sum, 2));

    sum = 0.0;
    for (int i = y + 1;  i < H; ++i) {
        for (int j = x + 1; j < W - x; ++j) {
            size_t pixelOffset = (i * image.getInfoHeader().bi_width + j) * bytesPerPixel;
            uint8_t componentValue = image.getData()[pixelOffset + bitShift] - expVal2;
            sum += static_cast<double>(componentValue);
        }
    }
    sigma2 = std::sqrt((1.0 / ((W * H) - 1)) * std::pow(sum, 2));

    sum = 0.0;
    double sum2 = 0.0;
    for (int i = 1; i < H - y; ++i) {
        for (int j = 1; j < W - x; ++j) {
            size_t pixelOffset = (i * image.getInfoHeader().bi_width + j) * bytesPerPixel;
            uint8_t componentValue = image.getData()[pixelOffset + bitShift] - expVal1;
            sum += static_cast<double>(componentValue);
        }
    }
    for (int i = y + 1;  i < H; ++i) {
        for (int j = x + 1; j < W - x; ++j) {
            size_t pixelOffset = (i * image.getInfoHeader().bi_width + j) * bytesPerPixel;
            uint8_t componentValue = image.getData()[pixelOffset + bitShift] - expVal1;
            sum2 += static_cast<double>(componentValue);
        }
    }
    auto mul = sum * sum2;
    auto M = (1.0 / (W * H)) * mul;
    r = M / (sigma1 * sigma2);

    return r;
}

double PSNR(bmp& srcImage, bmp& destImage, char mod) {
    if (srcImage.getData().size() != destImage.getData().size())
        std::cerr << "Impossible to count PSNR, because images's sizes don't equal";

    const size_t bytesPerPixel = srcImage.getInfoHeader().bi_bit_count / 8;

    uint8_t bitShift;
    switch (mod) {
        case 'r':
            bitShift = 0;
        break;
        case 'g':
            bitShift = 1;
        break;
        case 'b':
            bitShift = 2;
        break;
        default:
            return 0.0;
    }

    double sum {};
    for (int i = 0; i < srcImage.getInfoHeader().bi_height; ++i) {
        for (int j = 0; j < srcImage.getInfoHeader().bi_width; ++j) {
            size_t pixelOffset = (i * srcImage.getInfoHeader().bi_width + j) * bytesPerPixel;
            sum += pow((srcImage.getData()[pixelOffset + bitShift] - destImage.getData()[pixelOffset + bitShift]), 2);
        }
    }
    double result = 10 * std::log10(srcImage.getInfoHeader().bi_width * srcImage.getInfoHeader().bi_height * \
        std::pow(std::pow(2, 8) - 1, 2) / sum);
    return result;

}

void decimateImageEven(bmp& image, int num) {
    int originalWidth = image.getInfoHeader().bi_width;
    int originalHeight = image.getInfoHeader().bi_height;

    int newWidth = originalWidth / num;
    int newHeight = originalHeight / num;

    std::vector<uint8_t> decimatedImageData;

    for (int y = 0; y < originalHeight; y += num) {
        for (int x = 0; x < originalWidth; x += num) {
            int index = (y * originalWidth + x) * 3;

            decimatedImageData.push_back(image.getData()[index]);
            decimatedImageData.push_back(image.getData()[index + 1]);
            decimatedImageData.push_back(image.getData()[index + 2]);
        }
    }

    image.setWidth(newWidth);
    image.setHeight(newHeight);
    auto tmp = image.getInfoHeader().bi_size_image;
    image.setSize(newWidth * newHeight * 3);
    image.setImageSize(image.getFileHeader().bf_size - tmp + image.getInfoHeader().bi_size_image);

    image.setData(decimatedImageData);
    image.saveImage("../data/decimationEven.bmp", decimatedImageData);
}

void decimateImageAvg(bmp& image) {
    int originalWidth = image.getInfoHeader().bi_width;
    int originalHeight = image.getInfoHeader().bi_height;

    int newWidth = originalWidth / 2;
    int newHeight = originalHeight / 2;

    std::vector<uint8_t> decimatedImageData;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int index1 = ((2 * y) * originalWidth + (2 * x)) * 3; // left-up pixel
            int index2 = index1 + 3; // right-up pixel
            int index3 = index1 + originalWidth * 3; // left-bottom pixel
            int index4 = index3 + 3; // left bottom pixel


            uint8_t averageR = (image.getData()[index1] + image.getData()[index2] +
                                image.getData()[index3] + image.getData()[index4]) / 4;
            uint8_t averageG = (image.getData()[index1 + 1] + image.getData()[index2 + 1] +
                                image.getData()[index3 + 1] + image.getData()[index4 + 1]) / 4;
            uint8_t averageB = (image.getData()[index1 + 2] + image.getData()[index2 + 2] +
                                image.getData()[index3 + 2] + image.getData()[index4 + 2]) / 4;

            decimatedImageData.push_back(averageR);
            decimatedImageData.push_back(averageG);
            decimatedImageData.push_back(averageB);
        }
    }

    image.setWidth(newWidth);
    image.setHeight(newHeight);
    image.setSize(newWidth * newHeight * 3);
    image.setImageSize(image.getData().size() + sizeof(image.getInfoHeader()) + sizeof(image.getFileHeader()));

    image.setData(decimatedImageData);
    image.saveImage("../data/decimationAvg.bmp", decimatedImageData);
}

void restoreImage(bmp& image, int num) {
    int originalWidth = image.getInfoHeader().bi_width;
    int originalHeight = image.getInfoHeader().bi_height;

    int newWidth = originalWidth * num;
    int newHeight = originalHeight * num;

    std::vector<uint8_t> restoredImageData(newWidth * newHeight * 3);

    auto decimatedImageData = image.getData();

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int restoredIndex = (y * newWidth + x) * 3;

            // Если индекс четный
            if ((x % 2 == 0 && y % 2 == 0) || (x % 2 != 0 && y % 2 != 0)) {
                // Копируем значение пикселя из исходного изображения
                int originalX = x / num;
                int originalY = y / num;
                int originalIndex = (originalY * originalWidth + originalX) * 3;
                restoredImageData[restoredIndex] = decimatedImageData[originalIndex];
                restoredImageData[restoredIndex + 1] = decimatedImageData[originalIndex + 1];
                restoredImageData[restoredIndex + 2] = decimatedImageData[originalIndex + 2];
            }
                // Иначе, если индекс нечетный
            else {
                // Копируем значение пикселя из нового изображения
                // Предполагаем, что пиксели слева и сверху уже скопированы
                // (т.е. значение уже присутствует в restoredImageData)
                // Копируем значение соседнего пикселя
                restoredImageData[restoredIndex] = restoredImageData[restoredIndex];
                restoredImageData[restoredIndex + 1] = restoredImageData[restoredIndex + 1];
                restoredImageData[restoredIndex + 2] = restoredImageData[restoredIndex + 2];
            }
        }
    }

    image.setWidth(newWidth);
    image.setHeight(newHeight);
    image.setSize(newWidth * newHeight * 3);
    image.setImageSize(image.getFileHeader().bf_size - image.getInfoHeader().bi_size_image + image.getInfoHeader().bi_size_image);

    image.setData(restoredImageData);
    image.saveImage("../data/restored.bmp", restoredImageData);
}

double countProbability(bmp& image, char mod) {
    std::map<int, int> count_map;

    uint8_t bitShift;
    switch (mod) {
        case 'r':
            bitShift = 0;
            break;
        case 'g':
            bitShift = 1;
            break;
        case 'b':
            bitShift = 2;
            break;
        default:
            return 0.0;
    }
    for (int i {}; i < image.getInfoHeader().bi_height; ++i) {
        for (int j {}; j < image.getInfoHeader().bi_width; ++j) {
            uint8_t pixelIntensity = image.getData()[(i * image.getInfoHeader().bi_width + j) * 3];
            count_map[pixelIntensity]++;
        }
    }
    
}