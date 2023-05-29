#include "bmpcoder.h"
#include "bmploader.h"
#include "encodedimagedata.h"

#include <map>
#include <iostream>

bool BmpCoder::compress(const std::string& inFilePath) const
{
    bool result = false;

    EncodedImageData encodedImageData;

    BmpLoader loader;
    RawImageData rawDataImage = loader.readFromFile(inFilePath);
    const std::vector<std::byte>& bytes = rawDataImage.bytes();
    if (bytes.size() > 0) {
        const int width = rawDataImage.width();
        const int height = rawDataImage.height();
        std::cout << "image dimensions, w=" << width << ", h=" << height << std::endl;

        std::vector<std::byte> row;
        int counter = 0;
        for (std::size_t i=0; i<bytes.size(); ++i) {
            row.push_back(bytes.at(i));
            counter++;
            if (counter == width) {
                counter = 0;
                std::vector<std::byte> encodedRow = encodeRow(row);
                encodedImageData.addEncodedRow(encodedRow);
                row.clear();
            }
        }

        // todo implement processing
        result = true;
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
