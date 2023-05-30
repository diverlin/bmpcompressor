#include "barchloader.h"

#include <fstream>
#include <vector>
#include <iostream>

#include <cstdint>

#pragma pack(push, 1) // // Ensure the struct is packed without padding
struct BARCHHeader {
    // BARCHFILEHEADER
    char signature[2];         // File type signature ('BA' for BARCH)
    uint32_t fileSize;         // Size of the BARCH file in bytes

    // BARCHINFOHEADER
    uint32_t headerSize;    // Size of the BMPINFOHEADER structure in bytes
    int32_t width;          // Width of the image in pixels
    int32_t height;         // Height of the image in pixels
    int32_t rowIndexesSize; //
    int32_t dataSize;       //
};
#pragma pack(pop)

#ifdef PRINT_FILE_HEADERS
namespace {
void printHeader(const BARCHHeader& header)
{
    std::cout << "header.signature[0]=" << header.signature[0] << std::endl;
    std::cout << "header.signature[1]=" << header.signature[1] << std::endl;
    std::cout << "header.fileSize=" << header.fileSize << std::endl;

    std::cout << "header.headerSize=" << header.headerSize << std::endl;
    std::cout << "header.width=" << header.width << std::endl;
    std::cout << "header.height=" << header.height << std::endl;
    std::cout << "header.rowIndexesSize=" << header.rowIndexesSize << std::endl;
    std::cout << "header.dataSize=" << header.dataSize << std::endl;
}
}
#endif // PRINT_FILE_HEADERS

std::shared_ptr<EncodedImageData> BarchLoader::readFromFile(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (file.is_open()) {
        BARCHHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(BARCHHeader));

#ifdef PRINT_FILE_HEADERS
        std::cout << "readFromFile, header from file:" << filePath << std::endl;
        printHeader(header);
#endif // PRINT_FILE_HEADERS

        // check if the file has the correct BMP signature
        if (header.signature[0] == 'B' && header.signature[1] == 'A') {
            std::shared_ptr<EncodedImageData> encodedImageData(new EncodedImageData(header.dataSize, header.rowIndexesSize, header.width, header.height));

            file.read(reinterpret_cast<char*>(encodedImageData->m_rowIndexes.data()), header.rowIndexesSize);
            file.read(reinterpret_cast<char*>(encodedImageData->m_bytes.data()), header.dataSize);

            // debug
            std::cout << "encodedImageIndexes=" << encodedImageData->rowIndexes().size() << std::endl;
            std::cout << "encodedImageData=" << encodedImageData->bytes().size() << std::endl;
            // debug

            return encodedImageData;
        } else {
            error("invalid BMP file. Signature mismatch.");
        }

        file.close();
    } else {
        error("failed to open the file.");
    }

    return nullptr;
}

bool BarchLoader::writeToFile(const std::string& filePath, const EncodedImageData& encodedData)
{
    BARCHHeader header{};

    header.signature[0] = 'B';
    header.signature[1] = 'A';
    header.fileSize = sizeof(BARCHHeader) + encodedData.rowIndexes().size() + encodedData.bytes().size();
    header.width = encodedData.width();
    header.height = encodedData.height();
    header.rowIndexesSize = encodedData.rowIndexes().size();
    header.dataSize = encodedData.bytes().size();

#ifdef PRINT_FILE_HEADERS
    std::cout << "writeToFile, header to file:" << filePath << std::endl;
    printHeader(header);
#endif // PRINT_FILE_HEADERS

    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        error("Error opening file: " + filePath);
        return false;
    }

    // Write the BMP header
    file.write(reinterpret_cast<const char*>(&header), sizeof(BARCHHeader));

    // Write the row indexes
    file.write(reinterpret_cast<const char*>(encodedData.rowIndexes().data()), encodedData.rowIndexes().size());

    // Write the data
    file.write(reinterpret_cast<const char*>(encodedData.bytes().data()), encodedData.bytes().size());

    file.close();

    return true;
}

void BarchLoader::error(const std::string& error)
{
    m_errorMsg = error;
    std::cerr << error;
}
