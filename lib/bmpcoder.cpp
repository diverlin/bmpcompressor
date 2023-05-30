#include "bmpcoder.h"
#include "bmploader.h"
#include "barchloader.h"
#include <rawimagedata.h>
#include <encodedimagedata.h>

#include <map>
#include <iostream>

BmpCoder::BmpCoder(bool enableWhiteRowEncoding)
    :
    m_enableWhiteRowEncoding(enableWhiteRowEncoding)
{}

BmpCoder::~BmpCoder()
{

}

std::shared_ptr<RawImageData> BmpCoder::loadBmp(const std::string& filePath)
{
    BmpLoader loader;
    std::shared_ptr<RawImageData> data = loader.readFromFile(filePath);
    if (!loader.errorMsg().empty()) {
        error(loader.errorMsg());
    }
    return data;
}

std::shared_ptr<EncodedImageData> BmpCoder::loadBarch(const std::string& filePath)
{
    BarchLoader loader;
    std::shared_ptr<EncodedImageData> data = loader.readFromFile(filePath);
    if (!loader.errorMsg().empty()) {
        error(loader.errorMsg());
    }
    return data;
}


std::shared_ptr<EncodedImageData> BmpCoder::encode(const std::shared_ptr<RawImageData>& rawImageData)
{
    if (rawImageData) {
        if (rawImageData->isValid()) {
            const std::vector<std::byte>& bytes = rawImageData->bytes();
            const unsigned int width = rawImageData->width();
            const unsigned int height = rawImageData->height();
            std::shared_ptr<EncodedImageData> encodedImageData(new EncodedImageData{width, height});

            std::vector<std::byte> rowBuff;
            int counter = 0;
            for (std::size_t i=0; i<bytes.size(); ++i) {
                rowBuff.push_back(bytes.at(i));
                counter++;
                if (counter == utils::findNextDivisibleByFour(width)) {
                    counter = 0;
                    std::vector<std::byte> encodedRow = encodeRow(rowBuff, width);
                    encodedImageData->addEncodedRow(encodedRow);
                    rowBuff.clear();
                }
            }
            return encodedImageData;
        } else {
            error("invalid rawImageData structure, cannot encode");
        }
    } else {
        error("rawImageData is null, cannot encode");
    }
    return nullptr;
}

std::shared_ptr<RawImageData> BmpCoder::decode(const std::shared_ptr<EncodedImageData>& encodedImageData)
{
    if (encodedImageData) {
        if (encodedImageData->isValid()) {
            std::shared_ptr<RawImageData> decodedImageData(new RawImageData{encodedImageData->width(), encodedImageData->height()});

            const std::vector<std::byte>& rowIndexes = encodedImageData->rowIndexes();
            const std::vector<std::byte>& bytes = encodedImageData->bytes();

            std::size_t offset = 0;
            for (std::size_t i=0; i<=rowIndexes.size()-2; i+=2) {
                std::byte l = rowIndexes[i];
                std::byte r = rowIndexes[i+1];
                uint16_t rowSize = EncodedImageData::getDecodedIndex({l,r});
                std::vector<std::byte> encodedRow;
                for (std::size_t j=offset; j<offset+rowSize; ++j) {
                    encodedRow.push_back(bytes[j]);
                }
                offset += encodedRow.size();
                std::vector<std::byte> decodedRow = decodeRow(encodedRow, encodedImageData->width());
                decodedImageData->addDecodedRow(decodedRow);
            }

            return decodedImageData;
        }
        else {
            error("invalid encodedImageData structure, cannot decode");
        }
    } else {
        error("encodedImageData is null, cannot decode");
    }
    return nullptr;
}

bool BmpCoder::save(const std::string& filePath, const std::shared_ptr<RawImageData>& data)
{
    bool result = false;
    if (data) {
        BmpLoader loader;
        result = loader.writeToFile(filePath, *data);
        if (!loader.errorMsg().empty()) {
            error(loader.errorMsg());
        }
    } else {
        error("decodedImageData is null, cannot save");
    }
    return result;
}

bool BmpCoder::save(const std::string& filePath, const std::shared_ptr<EncodedImageData>& data)
{
    bool result = false;

    if (data) {
        BarchLoader loader;
        result = loader.writeToFile(filePath, *data);
        if (!loader.errorMsg().empty()) {
            error(loader.errorMsg());
        }
    } else {
        error("encodedImageData is null, cannot save");
    }

    return result;
}

bool BmpCoder::encode(const std::string& bmpFilePath, const std::string& barkFilePath)
{
    bool result = false;

    m_errorMsg = "";
    m_whiteRowsCounter = 0;

    BmpLoader bmpLoader;
    auto rawImageData = loadBmp(bmpFilePath);
    if (rawImageData) {
        auto encodedData = encode(rawImageData);
        if (encodedData) {
            result = save(barkFilePath, encodedData);
        }
    }

    return result;
}

bool BmpCoder::decode(const std::string& barchFilePath, const std::string& bmpFilePath)
{
    bool result = false;

    m_errorMsg = "";
    m_whiteRowsCounter = 0;

    auto encodedData = loadBarch(barchFilePath);
    if (encodedData) {
        auto decodedData = decode(encodedData);
        if (decodedData) {
            result = save(bmpFilePath, decodedData);
        }
    }

    return result;
}

std::vector<std::byte> BmpCoder::encodeRow(const std::vector<std::byte>& row, int imageWidth) const
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

    if (m_enableWhiteRowEncoding) {
        int white_counter = 0;
        for (std::byte b: row) {
            if (b == std::byte(0xff)) {
                white_counter++;
                if (white_counter >= imageWidth) { // rows include some padding with 0x00 values which prevents to interpret row as fully white
                    break;
                }
            }
        }
        if (white_counter >= imageWidth) {
            m_whiteRowsCounter++;
//            std::cout<<"white row counter=" << m_whiteRowsCounter << std::endl;
            encodedRow.clear();
        }
    }
    return std::move(encodedRow);
}

std::vector<std::byte> BmpCoder::decodeRow(const std::vector<std::byte>& encodedRow, int imageWidth) const
{
    std::vector<std::byte> decodedRow;

    if (m_enableWhiteRowEncoding) {
        if (encodedRow.empty()) {
            append(decodedRow, std::byte(0xff), imageWidth);
            if (utils::findNextDivisibleByFour(imageWidth) != imageWidth) {
                append(decodedRow, std::byte(0x00), utils::findNextDivisibleByFour(imageWidth)-imageWidth);
            }
            std::move(decodedRow);
        }
    }

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

void BmpCoder::error(const std::string& msg)
{
    m_errorMsg = msg;
    std::cerr << msg << std::endl;
}
