//
// Created by den_p on 3/4/2024.
//

#include "bmp.hpp"

#include <fstream>
#include <iostream>

bmp::bmp(const std::string &fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for reading." << std::endl;
        return;
    }
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (fileHeader.bf_type != 0x4D42) {
        std::cerr << "Unknown file's format" << std::endl;
        return;
    }
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    imageData.resize(infoHeader.bi_size_image + fileHeader.bf_off_bits);
    file.read(reinterpret_cast<char*>(imageData.data()), imageData.size());
    // std::cout << imageData.size() << std::endl;

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
    file.write(reinterpret_cast<char*>(data.data()), data.size());

    return true;
}
std::vector<uint8_t> bmp::convertRGBtoYCbCr()   {
        std::vector<uint8_t> ycbcrData;
        ycbcrData.reserve(imageData.size());

        for (size_t i = 0; i < imageData.size(); i += 3) {
            uint8_t r = imageData[i];
            uint8_t g = imageData[i + 1];
            uint8_t b = imageData[i + 2];


            uint8_t y = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
            uint8_t cb = static_cast<uint8_t>(0.5643 * (b - y) + 128);
            uint8_t cr = static_cast<uint8_t>(0.7132 * (r - y) + 128);


            ycbcrData.push_back(y);
            ycbcrData.push_back(cb);
            ycbcrData.push_back(cr);
        }


        saveImage("../data/YCbCr_component.bmp", ycbcrData);
    return ycbcrData;
}


void bmp::print_header() const {
    std::cout << std::hex <<
        fileHeader.bf_type << " " <<
        fileHeader.bf_size << " " <<
        fileHeader.bf_reserved1 << " " <<
        fileHeader.bf_reserved2 << " " <<
        fileHeader.bf_off_bits << " " << std::endl;

    std::cout << std::hex <<
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
    saveImage("../data/image_by_" + std::string(1, mod) + "_component.bmp", tmp_data);
    return tmp_data;
}
std::vector<uint8_t> bmp::saveFileByComponentYCbCr(const char &mod, std::vector<uint8_t> data) {
    std::vector tmp_data{data};
    switch (mod) {
        case 'Y':
            for (size_t i {}; i < tmp_data.size(); i += 3) {
                tmp_data[i    ] = 0x00;
                tmp_data[i + 1] = 0x00;
            }
        break;
        case 'B':
            for (size_t i {}; i < tmp_data.size(); i += 3) {
                tmp_data[i    ] &= 0x00;
                tmp_data[i + 2] &= 0x00;
            }
        break;
        case 'R':
            for (size_t i {}; i < tmp_data.size(); i += 3) {
                tmp_data[i + 1] &= 0x00;
                tmp_data[i + 2] &= 0x00;
            }
        break;
        default:
            break;
    }
    saveImage("../data/image_by_" + std::string(1, mod) + "_component.bmp", tmp_data);
    return tmp_data;
}