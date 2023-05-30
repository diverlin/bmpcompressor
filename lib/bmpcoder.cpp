#include "bmpcoder.h"
#include "bmploader.h"
#include "barchloader.h"
#include <rawimagedata.h>
#include <encodedimagedata.h>

#include <map>
#include <iostream>

BmpCoder::~BmpCoder()
{

}

bool BmpCoder::load(const std::string& filePath)
{
    bool result = false;
    if (utils::endsWith(filePath, ".bmp")) {
        BmpLoader loader;
        m_rawImageData = loader.readFromFile(filePath);
    } else if (utils::endsWith(filePath, ".barch")) {
        BarchLoader loader;
        m_encodedImageData = loader.readFromFile(filePath);
    }
}

bool BmpCoder::encode()
{
    bool result = false;

    if (m_rawImageData) {
        if (m_rawImageData->isValid()) {
            const std::vector<std::byte>& bytes = m_rawImageData->bytes();
            const unsigned int width = m_rawImageData->width();
            const unsigned int height = m_rawImageData->height();
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
            result = true;
        } else {
            std::cerr << "invalid rawImageData structure, cannot encode" << std::endl;
        }
    } else {
        std::cerr << "rawImageData is null, cannot encode" << std::endl;
    }
    return result;
}

bool BmpCoder::decode()
{
    bool result = false;

    if (m_encodedImageData) {
        if (m_encodedImageData->isValid()) {
            m_decodedImageData = std::shared_ptr<DecodedImageData>(new DecodedImageData{m_encodedImageData->width(), m_encodedImageData->height()});

            const std::vector<std::byte>& rowIndexes = m_encodedImageData->rowIndexes();
            const std::vector<std::byte>& bytes = m_encodedImageData->bytes();

            std::size_t offset = 0;
            for (std::size_t i=0; i<=rowIndexes.size()-2; i+=2) {
                std::byte l = rowIndexes[i];
                std::byte r = rowIndexes[i+1];
                uint16_t rowSize = EncodedImageData::getDecodedIndex({l,r});
                std::vector<std::byte> encodedRow;
                for (std::size_t j=offset; j<offset+rowSize; ++j) {
                    encodedRow.push_back(bytes[j]);
                }
                std::vector<std::byte> decodedRow = decodeRow(encodedRow);
                m_decodedImageData->addDecodedRow(decodedRow);
            }
        }
        else {
            std::cerr << "invalid encodedImageData structure, cannot decode" << std::endl;
        }
    } else {
        std::cerr << "encodedImageData is null, cannot decode" << std::endl;
    }
    return result;
}

bool BmpCoder::save(const std::string& filePath)
{
    bool result = false;
    if (utils::endsWith(filePath, ".bmp")) {
        if (m_decodedImageData) {
            BmpLoader loader;
            result = loader.writeToFile(filePath, *m_decodedImageData);
        } else {
            std::cerr << "decodedImageData is null, cannot save" << std::endl;
        }
    } else if (utils::endsWith(filePath, ".barch")) {
        if (m_encodedImageData) {
        BarchLoader loader;
            loader.writeToFile(filePath, *m_encodedImageData);
        } else {
            std::cerr << "encodedImageData is null, cannot save" << std::endl;
        }
    }
}

bool BmpCoder::encode(const std::string& bmpFilePath, const std::string& barkFilePath)
{
    bool result = false;

    BmpLoader bmpLoader;
    m_rawImageData = bmpLoader.readFromFile(bmpFilePath);
    if (encode()) {
        if (m_encodedImageData) {
            BarchLoader barchLoader;
            result = barchLoader.writeToFile(barkFilePath, *m_encodedImageData);
        }
    }

    return result;
}

bool BmpCoder::decode(const std::string& barkFilePath, const std::string& bmpFilePath)
{
    bool result = false;
    BarchLoader barchLoader;

    m_encodedImageData = barchLoader.readFromFile(barkFilePath);
    if (m_encodedImageData->isValid()) {
        if (decode()) {
            if (m_decodedImageData) {
                BmpLoader bmpLoader;
                result = bmpLoader.writeToFile(bmpFilePath, *m_decodedImageData);
            }
        }
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
