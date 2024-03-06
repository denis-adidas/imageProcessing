
#ifndef BMP_HPP
#define BMP_HPP

#include <cstdint>
#include <string>
#include <vector>


class bmp {
#pragma pack(push)
#pragma pack(1)
    struct BMPFileHeader {
        uint16_t bf_type; // must be 0x4D42
        uint32_t bf_size;
        uint16_t bf_reserved1;
        uint16_t bf_reserved2;
        uint32_t bf_off_bits;
    } fileHeader {};
    struct BMPInfoHeader {
        uint32_t bi_size;
        int32_t bi_width;
        int32_t bi_height;
        uint16_t bi_planes;
        uint16_t bi_bit_count;
        uint32_t bi_compression;
        uint32_t bi_size_image;
        int32_t bi_x_pixels_per_meter;
        int32_t bi_y_pixels_per_meter;
        uint32_t bi_colors_used;
        uint32_t bi_colors_important;
    } infoHeader {};
#pragma pack(pop)
    std::vector<uint8_t> palette;
    std::vector<uint8_t> imageData;
public:
    bmp()= default;
    explicit bmp(const std::string& fileName);
    bool saveImage(const std::string &filename, std::vector<uint8_t> data);
    std::vector<uint8_t> convertRGBtoYCbCr();
    std::vector<uint8_t> saveFileByComponent(const char& mod);
    std::vector<uint8_t> saveFileByComponentYCbCr(const char &mod, std::vector<uint8_t> data);
    void rotate(int rotateCount);

    void print_header() const;

    std::vector<uint8_t> getData() {
        return imageData;
    };
    BMPFileHeader getFileHeader() {
        return fileHeader;
    }
    BMPInfoHeader getInfoHeader() {
        return infoHeader;
    }
};

#endif
