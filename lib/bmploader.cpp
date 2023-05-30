#include "bmploader.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <memory>

#include <cstdint>

#pragma pack(push, 1) // // Ensure the struct is packed without padding
struct BMPHeader {
    // BITMAPFILEHEADER
    char signature[2];      // File type signature ('BM' for BMP)
    uint32_t fileSize;      // Size of the BMP file in bytes
    uint32_t reserved;      // Reserved, set to 0
    uint32_t dataOffset;    // Offset of the pixel data in bytes

    // BITMAPINFOHEADER
    uint32_t headerSize;    // Size of the BMPINFOHEADER structure in bytes
    int32_t width;          // Width of the image in pixels
    int32_t height;         // Height of the image in pixels
    uint16_t planes;        // Number of color planes, typically 1
    uint16_t bitsPerPixel;  // Number of bits per pixel
    uint32_t compression;   // Compression method used (0 for uncompressed)
    uint32_t imageSize;     // Size of the raw image data in bytes (including padding)
    int32_t xPixelsPerMeter; // Horizontal resolution (pixels per meter)
    int32_t yPixelsPerMeter; // Vertical resolution (pixels per meter)
    uint32_t totalColors;    // Number of colors used in the image (0 for maximum)
    uint32_t importantColors; // Number of important colors (0 for all)

    // Additional fields or padding, if any
};
#pragma pack(pop)

#ifdef PRINT_FILE_HEADERS
namespace {
void printHeader(const BMPHeader& header)
{
    std::cout << "header.signature[0]=" << header.signature[0] << std::endl;
    std::cout << "header.signature[1]=" << header.signature[1] << std::endl;
    std::cout << "header.fileSize=" << header.fileSize << std::endl;
    std::cout << "header.dataOffset=" << header.dataOffset << std::endl;

    std::cout << "header.headerSize=" << header.headerSize << std::endl;
    std::cout << "header.width=" << header.width << std::endl;
    std::cout << "header.height=" << header.height << std::endl;
    std::cout << "header.planes=" << header.planes << std::endl;
    std::cout << "header.bitsPerPixel=" << header.bitsPerPixel << std::endl;
    std::cout << "header.compression=" << header.compression << std::endl;
    std::cout << "header.imageSize=" << header.imageSize << std::endl;
    std::cout << "header.xPixelsPerMeter=" << header.xPixelsPerMeter << std::endl;
    std::cout << "header.yPixelsPerMeter=" << header.yPixelsPerMeter << std::endl;
    std::cout << "header.totalColors=" << header.totalColors << std::endl;
    std::cout << "header.importantColors=" << header.importantColors << std::endl << std::endl;
}
}
#endif // PRINT_FILE_HEADERS

BmpLoader::BmpLoader(int bitsPerPixel)
    :
      m_bitsPerPixel(bitsPerPixel)
{
    if ((bitsPerPixel != 8) && (bitsPerPixel != 24)) {
        std::cerr << "the bitsPerPixel=" << bitsPerPixel << " format is not supported, please use 8 or 24 values. The modle may work not properly due to wrong header intiialization" << std::endl;
    }
}

std::shared_ptr<RawImageData> BmpLoader::readFromFile(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (file.is_open()) {
        BMPHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

#ifdef PRINT_FILE_HEADERS
        std::cout << "readFromFile, header from file:" << filePath << std::endl;
        printHeader(header);
#endif // PRINT_FILE_HEADERS

        // check if the file has the correct BMP signature
        if (header.signature[0] == 'B' && header.signature[1] == 'M') {
            if (header.bitsPerPixel == m_bitsPerPixel) {
                std::shared_ptr<RawImageData> rawImageData(new RawImageData(header.width, header.height, header.imageSize));

                file.seekg(header.dataOffset, std::ios::beg);
                file.read(reinterpret_cast<char*>(rawImageData->m_bytes.data()), header.imageSize);

#ifdef PRINT_FILE_HEADERS
                std::cout << "rawImageData=" << rawImageData->bytes().size() << std::endl;
#endif // PRINT_FILE_HEADERS

                return rawImageData;
            } else {
                std::cout << "invalid BMP file format. Expected " << m_bitsPerPixel << " bits per pixel. But got=" << header.bitsPerPixel << std::endl;
            }
        } else {
            std::cout << "invalid BMP file. Signature mismatch." << std::endl;
        }

        file.close();
    } else {
        std::cout << "failed to open the file." << std::endl;
    }

    return nullptr;
}

bool BmpLoader::writeToFile(const std::string& filePath, const RawImageData& rawData)
{
    BMPHeader header{};

    int colorPalleteOffset = 0;
    if (m_bitsPerPixel == 8) {
        colorPalleteOffset = 256 * sizeof(uint32_t);
    }

    header.signature[0] = 'B';
    header.signature[1] = 'M';
    header.fileSize = sizeof(BMPHeader) + colorPalleteOffset + rawData.bytes().size();
    header.reserved = 0;
    header.dataOffset = sizeof(BMPHeader) + colorPalleteOffset;
    header.headerSize = sizeof(BMPHeader)-14; // Subtract the size of the file signature and file size fields
    header.width = rawData.width();
    header.height = rawData.height();
    header.planes = 1;
    header.bitsPerPixel = m_bitsPerPixel;
    header.compression = 0; // no compression
    header.imageSize = rawData.bytes().size(); // can be set to 0 for uncompressed images
    header.xPixelsPerMeter = 0;
    header.yPixelsPerMeter = 0;
    if (m_bitsPerPixel == 8) {
        header.totalColors = 256;
    } else {
        header.totalColors = 0;
    }
    header.importantColors = 0;

#ifdef PRINT_FILE_HEADERS
    std::cout << "writeToFile, header to file:" << filePath << std::endl;
    printHeader(header);
#endif // PRINT_FILE_HEADERS

    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return false;
    }

    // Write the BMP header
    file.write(reinterpret_cast<const char*>(&header), sizeof(BMPHeader));

    // Write the color palette
    if (colorPalleteOffset > 0) {
        for (int i = 0; i < 256; ++i) {
            uint32_t color = (i << 16) | (i << 8) | i;
            file.write(reinterpret_cast<const char*>(&color), sizeof(uint32_t));
        }
    }

    // Write the grayscale pixel data
    file.write(reinterpret_cast<const char*>(rawData.bytes().data()), rawData.bytes().size());

    file.close();

    return true;
}
