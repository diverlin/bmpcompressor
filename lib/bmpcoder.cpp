#include "bmpcoder.h"
#include "bmploader.h"
#include "barchloader.h"
#include "encodedimagedata.h"

#include <map>
#include <iostream>

bool BmpCoder::encode(const std::string& bmpFilePath, const std::string& barkFilePath) const
{
    bool result = false;

    BmpLoader bmpLoader;
    RawImageData rawDataImage = bmpLoader.readFromFile(bmpFilePath);
    const std::vector<std::byte>& bytes = rawDataImage.bytes();
    if (rawDataImage.isValid()) {
        const unsigned int width = rawDataImage.width();
        const unsigned int height = rawDataImage.height();
        EncodedImageData encodedImageData{width, height};

        std::vector<std::byte> rowBuff;
        unsigned int counter = 0;
        for (std::size_t i=0; i<bytes.size(); ++i) {
            rowBuff.push_back(bytes.at(i));
            counter++;
            if (counter == width) {
                counter = 0;
                std::vector<std::byte> encodedRow = encodeRow(rowBuff);
                encodedImageData.addEncodedRow(encodedRow);
                rowBuff.clear();
            }
        }
        BarchLoader barchLoader;
        result = barchLoader.writeToFile(barkFilePath, encodedImageData);
    } else {
        std::cerr << "invalid rawDataImage structure, cannot encode" << std::endl;
    }

    return result;
}

bool BmpCoder::decode(const std::string& barkFilePath, const std::string& bmpFilePath) const
{
    bool result = false;
    BarchLoader barchLoader;

    EncodedImageData encodedImageData = barchLoader.readFromFile(barkFilePath);
    if (encodedImageData.isValid()) {
        DecodedImageData decodedImageData{encodedImageData.width(), encodedImageData.height()};

        const std::vector<std::byte>& rowIndexes = encodedImageData.rowIndexes();
        const std::vector<std::byte>& bytes = encodedImageData.bytes();

        std::size_t offset = 0;
        for (std::size_t i=0; i<rowIndexes.size()-2; i+=2) {
            std::byte l = rowIndexes[i];
            std::byte r = rowIndexes[i+1];
            uint16_t rowSize = EncodedImageData::getDecodedIndex({l,r});
            std::vector<std::byte> encodedRow;
            for (std::size_t j=offset; j<offset+rowSize; ++j) {
                encodedRow.push_back(bytes[j]);
            }
            std::vector<std::byte> decodedRow = decodeRow(encodedRow);
            decodedImageData.addDecodedRow(decodedRow);
        }

        BmpLoader bmpLoader;
        result = bmpLoader.writeToFile(bmpFilePath, decodedImageData);
    }
    else {
        std::cerr << "invalid encodedImageData structure, cannot decode" << std::endl;
    }
    return result;
}

std::vector<std::byte> BmpCoder::encodeRow(const std::vector<std::byte>& row) const
{
    std::vector<std::byte> encodedRow;
    encodedRow.reserve(row.size());

    static std::vector<int> offsets{0,1,2,3};
    int handledSymbolsCounter = 0;
    for (std::size_t i=0; i<=row.size()-4; i+=4) {
        int whiteCount = 0;
        int blackCount = 0;

        for (int offset: offsets) {
            std::byte pixel = row[i+offset];
            if (pixel == std::byte(0x00)) {
                blackCount++;
            } else if (pixel == std::byte(0xff)) {
                whiteCount++;
            }
        }
        if (whiteCount == 4) {
            encodedRow.push_back(std::byte(0x00));
        } else if (blackCount == 4) {
            encodedRow.push_back(std::byte(0x02));
        } else {
            encodedRow.push_back(std::byte(0x03));
            for (int offset: offsets) {
                std::byte pixel = row[i+offset];
                encodedRow.push_back(pixel);
            }
        }
        handledSymbolsCounter+=4;
    }

    // handle tail if exists, as adding 11 and than colors like they are
    if (handledSymbolsCounter < row.size()) {
        encodedRow.push_back(std::byte(0x03));

        std::vector<std::byte> tail(row.begin()+handledSymbolsCounter, row.end());

        for (std::byte ch: tail) {
            encodedRow.push_back(ch);
        }
    }

    return std::move(encodedRow);
}

std::vector<std::byte> BmpCoder::decodeRow(const std::vector<std::byte>& encodedRow) const
{
    std::vector<std::byte> decodedRow;
    decodedRow.reserve(3*encodedRow.size());

    bool isUnpackingVarColors = false;
    int varColorCounter = 0;
    for (std::byte ch: encodedRow) {
        if (!isUnpackingVarColors) {
            if (ch == std::byte(0x00)) {
                append(decodedRow, std::byte(0xff), 4); // append 4 bytes of white
            } else if (ch == std::byte(0x02)) {
                append(decodedRow, std::byte(0x00), 4); // append 4 bytes of black
            } else if (ch == std::byte(0x03)) {
                isUnpackingVarColors = true;
                varColorCounter = 0;
            }
        } else {
            append(decodedRow, ch);
            varColorCounter++;
            if (varColorCounter==4) {
                isUnpackingVarColors = false;
            }
        }
    }

    return std::move(decodedRow);
}

void BmpCoder::append(std::vector<std::byte>& src, std::byte ch, int repeat) const
{
    while (repeat > 0) {
        src.push_back(ch);
        repeat--;
    }
}
