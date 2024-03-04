#include "util.hpp"

bool readBinaryFile(const std::string &filename, std::string& buffer) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for reading." << std::endl;
        return false;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char *>(buffer.data()), fileSize)) {
        std::cerr << "Failed to read file." << std::endl;
        return false;
    }

    return true;
}
bool writeBinaryFile(const std::string &filename, const std::string& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for writing." << std::endl;
        return false;
    }

    if (!file.write(reinterpret_cast<const char *>(data.data()), data.size())) {
        std::cerr << "Failed to write file." << std::endl;
        return false;
    }

    return true;
}


std::string readBMPFile(const std::string &filename, std::string& buffer) {
    readBinaryFile(filename, buffer);
    std::string pixelData = buffer.substr(54);

    return pixelData;
}

std::string saveFileByComponent(const std::string& pixelData, const std::string& header, const char& mod) {
    std::string tmp_data = pixelData;

    switch (mod) {
        case 'r':
            for (size_t i {}; i < tmp_data.size(); i += 3) {
                tmp_data[i    ] &= 0x00;
                tmp_data[i + 1] &= 0x00;
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

    writeBinaryFile("../data/image_by_" + std::string(1, mod) + "_component.bmp", header + tmp_data);

    return tmp_data;
}