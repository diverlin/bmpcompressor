#include "bmpreader.h"

#include <fstream>
#include <vector>
#include <iostream>

#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t horizontalResolution;
    int32_t verticalResolution;
    uint32_t colors;
    uint32_t importantColors;
};
#pragma pack(pop)

bool BmpLoader::readFromFile(const std::string& inFilePath)
{
    m_rawData.clear();

    bool result = false;

    std::ifstream file(inFilePath, std::ios::binary);

    if (file.is_open()) {
        BMPHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

        // check if the file has the correct BMP signature
        if (header.signature[0] == 'B' && header.signature[1] == 'M') {
            // check if the file is 8 bits per pixel (GS)
            if (header.bitsPerPixel == 8) {
                // calculate the size of the image data
                int dataSize = header.fileSize - header.dataOffset;

                // read the image data
                unsigned char* data = new unsigned char[header.width*header.height*3];
                file.read(reinterpret_cast<char*>(data), dataSize);

                m_rawData.width = header.width;
                m_rawData.height = header.height;
                m_rawData.data = data;
                m_rawData.dataSize = header.imageSize;
                result = true;
            } else {
                std::cout << "invalid BMP file format. Expected 8 bits per pixel. But got=" << header.bitsPerPixel << std::endl;
            }
        } else {
            std::cout << "invalid BMP file. Signature mismatch." << std::endl;
        }

        file.close();
    } else {
        std::cout << "failed to open the file." << std::endl;
    }

    return result;
}

bool BmpLoader::writeToFile(const std::string& filePath, const RawImageData& rawData)
{
    BMPHeader header{};
    header.signature[0] = 'B';
    header.signature[1] = 'M';
    header.fileSize = sizeof(BMPHeader) + rawData.size();
    header.dataOffset = sizeof(BMPHeader);
    header.headerSize = 40;
    header.width = rawData.width;
    header.height = rawData.height;
    header.planes = 1;
    header.bitsPerPixel = 8; // 8 bits per pixel
    header.compression = 0; // no compression
    header.imageSize = 0; // can be set to 0 for uncompressed images
    header.horizontalResolution = 0;
    header.verticalResolution = 0;
    header.colors = 0; // all colors are used
    header.importantColors = 0; // all colors are important

    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return false;
    }

    file.write(reinterpret_cast<const char*>(&header), sizeof(BMPHeader));
    file.write(reinterpret_cast<const char*>(rawData.data), rawData.size());

    file.close();

    return true;
}

