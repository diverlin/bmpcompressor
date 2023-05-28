#include "bmpcoder.h"
#include "bmpreader.h"

#include <map>
#include <iostream>

bool BmpCoder::compress(const std::string& inFilePath) const
{
    bool result = false;

    std::map<int, std::vector<unsigned char>> rowsMap;
    std::map<int, std::vector<unsigned char>> encodedRowsMap;
    std::size_t currentRowNum = 0;

    BmpReader reader;
    if (reader.readFromFile(inFilePath)) {
        const RawImageData& data = reader.data();
        const int width = data.width;
        std::cout << "image dimensions, w=" << data.width << ", h=" << data.height << std::endl;
        int counter = 0;
        for (std::size_t i=0; i<data.width * data.height; ++i) {
            rowsMap[currentRowNum].push_back(data.data[i]);
            counter++;
            if (counter == width) {
                currentRowNum++;
                counter = 0;
            }
        }

        auto it = rowsMap.begin();
        for (; it != rowsMap.end(); ++it) {
            const std::vector<unsigned char>& vec = it->second;
            encodedRowsMap[it->first] = encodeRow(vec);
            auto decoded = decodeRow(encodedRowsMap[it->first]);
            std::cout << "encoded length =" << encodedRowsMap[it->first].size() << ", decoded len=" << decoded.size() << std::endl;
        }

        std::cout << "rows num=" << currentRowNum << std::endl;
        std::cout << "elements in first row=" << rowsMap[0].size() << std::endl;
        std::cout << "elements in last row=" << rowsMap[data.height-1].size() << std::endl;


        // todo implement processing
        result = true;
    }

    return result;
}

std::vector<unsigned char> BmpCoder::encodeRow(const std::vector<unsigned char>& row) const
{
    std::vector<unsigned char> encodedRow;
    encodedRow.reserve(row.size());

    static std::vector<int> offsets{0,1,2,3};
    int handledSymbolsCounter = 0;
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
            encodedRow.push_back(static_cast<unsigned char>(0x00));
        } else if (blackCount == 4) {
            encodedRow.push_back(static_cast<unsigned char>(0x02));
        } else {
            encodedRow.push_back(static_cast<unsigned char>(0x03));
            for (int offset: offsets) {
                unsigned char pixel = row[i+offset];
                encodedRow.push_back(pixel);
            }
        }
        handledSymbolsCounter+=4;
    }

    // handle tail if exists, as adding 11 and than colors like they are
    if (handledSymbolsCounter < row.size()) {
        encodedRow.push_back(static_cast<unsigned char>(0x03));

        std::vector<unsigned char> tail(row.begin()+handledSymbolsCounter, row.end());

        for (unsigned char ch: tail) {
            encodedRow.push_back(ch);
        }
    }

    return std::move(encodedRow);
}

std::vector<unsigned char> BmpCoder::decodeRow(const std::vector<unsigned char>& encodedRow) const
{
    std::vector<unsigned char> decodedRow;
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

void BmpCoder::append(std::vector<unsigned char>& src, unsigned char ch, int repeat) const
{
    while (repeat > 0) {
        src.push_back(ch);
        repeat--;
    }
}
