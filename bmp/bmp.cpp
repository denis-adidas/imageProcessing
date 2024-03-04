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
