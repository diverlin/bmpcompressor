#include "bmpcompressor.h"
#include "bmploader.h"

#include <bitset>
#include <iostream>

bool BmpCompressor::compress(const std::string& inFilePath) const
{
    bool result = false;

    BmpLoader loader;
    if (loader.load(inFilePath)) {
        const RawImageData& data = loader.data();
        // todo implement processing
        result = false;
    }

    return result;
}

std::string BmpCompressor::encodeRow(const std::string& row) const
{
    std::string encodedRow;
    encodedRow.reserve(row.size());

    static std::vector<int> offsets{0,1,2,3};
    for (std::size_t i=0; i<=row.size()-4; i+=4) {
        int whiteCount = 0;
        int blackCount = 0;

        for (int offset: offsets) {
            unsigned char pixel = row[i+offset];
            if (pixel == 0x00) {
                blackCount++;
            } else if (pixel == 0xff) {
                whiteCount++;
            }
        }
        if (whiteCount == 4) {
            encodedRow += "0";
        } else if (blackCount == 4) {
            encodedRow += "10";
        } else {
            encodedRow += "11";
            for (int offset: offsets) {
                unsigned char pixel = row[i+offset];
                std::bitset<8> binary(pixel);
                encodedRow += binary.to_string();
            }
        }
    }

    std::cout << "encoded = " << encodedRow << std::endl;

    return std::move(encodedRow);
}

std::string BmpCompressor::decodeRow(const std::string&) const
{
    std::string decodedRow;

    // TODO: implement me

    return std::move(decodedRow);
}
