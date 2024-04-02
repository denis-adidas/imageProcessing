//
// Created by den_p on 3/4/2024.
//

#include "bmp.hpp"

#include <fstream>
#include <iostream>
#include <utility>

bmp::bmp(const std::string &fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for reading." << std::endl;
        return;
    }
    if (file) {
        file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
        if (fileHeader.bf_type != 0x4D42) {
            std::cerr << "Unknown file's format" << std::endl;
            return;
        }
        file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

        file.seekg(sizeof(infoHeader) + sizeof(fileHeader), std::ios::beg);
        palette.resize(fileHeader.bf_off_bits - sizeof(infoHeader) - sizeof(fileHeader));
        file.read(reinterpret_cast<char*>(palette.data()), palette.size());


        imageData.resize(infoHeader.bi_size_image);
        file.read(reinterpret_cast<char*>(imageData.data()), imageData.size());

        // std::cout << imageData[0] << std::endl;
    }
    file.close();
}

bool bmp::saveImage(const std::string &filename, std::vector<uint8_t> data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for writing." << std::endl;
        return false;
    }
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    file.write(reinterpret_cast<char*>(palette.data()), palette.size());
    file.write(reinterpret_cast<char*>((data).data()), data.size());

    return true;
}
std::vector<uint8_t> bmp::convertRGBtoYCbCr()   {
        std::vector<uint8_t> YData;
        std::vector<uint8_t> CbData;
        std::vector<uint8_t> CrData;
        std::vector<uint8_t> ycbcrData;

        YData.reserve(imageData.size());
        CbData.reserve(imageData.size());
        CrData.reserve(imageData.size());

        for (size_t i {}; i < imageData.size(); i += 3) {
            uint8_t r = imageData[i + 2];
            uint8_t g = imageData[i + 1];
            uint8_t b = imageData[i];


            int y = static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b);
            int cb = static_cast<int>(0.5643 * (b - y) + 128);
            int cr = static_cast<int>(0.7132 * (r - y) + 128);

            y = std::min(std::max(y, 0), 255);
            cb = std::min(std::max(cb, 0), 255);
            cr = std::min(std::max(cr, 0), 255);


            YData.emplace_back(y);
            YData.emplace_back(y);
            YData.emplace_back(y);


            CbData.emplace_back(cb);
            CbData.emplace_back(cb);
            CbData.emplace_back(cb);


            CrData.emplace_back(cr);
            CrData.emplace_back(cr);
            CrData.emplace_back(cr);

            ycbcrData.emplace_back(y);
            ycbcrData.emplace_back(cb);
            ycbcrData.emplace_back(cr);
        }

        saveImage("../data/Y_component.bmp", YData);
        saveImage("../data/Cb_component.bmp", CbData);
        saveImage("../data/Cr_component.bmp", CrData);
    return ycbcrData;
}

std::vector<uint8_t> bmp::convertYCbCrtoRGB(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> RGBImage;
    RGBImage.reserve(data.size());

    for (size_t i {}; i < data.size(); i += 3) {
        uint8_t Y = data[i];
        uint8_t Cb = data[i + 1];
        uint8_t Cr = data[i + 2];

        int r = static_cast<int>(Y + 1.402 * (Cr - 128));
        int g = static_cast<int>(Y - 0.344 * (Cb - 128) - 0.714 * (Cr - 128));
        int b = static_cast<int>(Y + 1.772 * (Cb - 128));

        r = std::min(std::max(r, 0), 255);
        g = std::min(std::max(g, 0), 255);
        b = std::min(std::max(b, 0), 255);

        RGBImage.push_back(b);
        RGBImage.push_back(g);
        RGBImage.push_back(r);
    }

    saveImage("../data/restoreImage.bmp", RGBImage);
    return RGBImage;
}



void bmp::print_header() const {
    std::cout << std::hex <<
        fileHeader.bf_type << " " <<
        fileHeader.bf_size << " " <<
        fileHeader.bf_reserved1 << " " <<
        fileHeader.bf_reserved2 << " " <<
        fileHeader.bf_off_bits << " " << std::endl;

    std::cout  <<
        infoHeader.bi_size << " " <<
        infoHeader.bi_width << " " <<
        infoHeader.bi_height << " " <<
        infoHeader.bi_planes << " " <<
        infoHeader.bi_bit_count << " " <<
        infoHeader.bi_compression << " " <<
        infoHeader.bi_size_image << " " <<
        infoHeader.bi_x_pixels_per_meter << " " <<
        infoHeader.bi_y_pixels_per_meter << " " <<
        infoHeader.bi_colors_used << " " <<
        infoHeader.bi_colors_important << " " << std::endl;
}
std::vector<uint8_t> bmp::saveFileByComponent(const char &mod) {
    std::vector tmp_data{imageData};
    switch (mod) {
        case 'r':
            for (size_t i {}; i < tmp_data.size(); i += 3) {
                tmp_data[i    ] = 0x00;
                tmp_data[i + 1] = 0x00;
            }
        break;
        case 'g':
            for (size_t i {}; i < tmp_data.size(); i += 3) {
                tmp_data[i    ] &= 0x00;
                tmp_data[i + 2] &= 0x00;
            }
        break;
        case 'b':
            for (size_t i {}; i < tmp_data.size(); i += 3) {
                tmp_data[i + 1] &= 0x00;
                tmp_data[i + 2] &= 0x00;
            }
        break;
        default:
            break;
    }

    saveImage("../data2/image_by_" + std::string(1, mod) + "_component.bmp", tmp_data);
    return tmp_data;
}

void bmp::rotate(int rotateCount) {
    auto height = infoHeader.bi_height;
    auto width = infoHeader.bi_width;
    const size_t bytesPerPixel = infoHeader.bi_bit_count / 8;

    std::vector<uint8_t> tmp_data(imageData.size());


    for (int j = 0; j < width; ++j) {
        for (int i = height - 1; i >= 0; --i) {
            size_t newIndex = (j * height + (height - 1 - i)) * bytesPerPixel;
            size_t oldIndex = (i * width + j) * bytesPerPixel;

            for (size_t k = 0; k < bytesPerPixel; ++k) {
                tmp_data[newIndex + k] = imageData[oldIndex + k];
            }
        }
    }

    std::swap(infoHeader.bi_width, infoHeader.bi_height);

    imageData = tmp_data;
}
