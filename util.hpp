#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

bool readBinaryFile(const std::string &filename, std::string& buffer);
bool writeBinaryFile(const std::string &filename, const std::string& data);

std::string readBMPFile(const std::string &filename, std::string& buffer);

std::string saveFileByComponent(const std::string& pixelData, const std::string& header, const char& mod);

#endif //UTIL_HPP
