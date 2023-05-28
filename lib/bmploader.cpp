#include "bmploader.h"
#include <fstream>
#include <vector>

#include <iostream>

namespace {
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
} // namespace

bool BmpLoader::load(const std::string& inFilePath)
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


