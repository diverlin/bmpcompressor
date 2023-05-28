#include "bmpcompressor.h"
#include "bmploader.h"

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
            encodedRow += static_cast<unsigned char>(0x00);
        } else if (blackCount == 4) {
            encodedRow += static_cast<unsigned char>(0x02);
        } else {
            encodedRow += static_cast<unsigned char>(0x03);
            for (int offset: offsets) {
                unsigned char pixel = row[i+offset];
                encodedRow += pixel;
            }
        }
    }

    return std::move(encodedRow);
}

std::string BmpCompressor::decodeRow(const std::string& encodedRow) const
{
    std::string decodedRow;
    decodedRow.reserve(3*encodedRow.size());

    bool isUnpackingVarColors = false;
    int varColorCounter = 0;
    for (unsigned char ch: encodedRow) {
        if (!isUnpackingVarColors) {
            if (ch == 0x00) {
                append(decodedRow, 0xff, 4); // append 4 bytes of white
            } else if (ch == 0x02) {
                append(decodedRow, 0x00, 4); // append 4 bytes of black
            } else if (ch == 0x03) {
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

void BmpCompressor::append(std::string& src, unsigned char ch, int repeat) const
{
    while (repeat > 0) {
        src += ch;
        repeat--;
    }
}
