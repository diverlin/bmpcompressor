#ifndef BMPLOADER_H
#define BMPLOADER_H

#include <string>

#include "rawimagedata.h"

class BmpLoader {
public:
    BmpLoader(int bitsPerPixel = 8):m_bitsPerPixel(bitsPerPixel) {}
    ~BmpLoader()=default;

    void setExpectedBitsPerPixel(int bitsPerPixel) { m_bitsPerPixel = bitsPerPixel; }

    RawImageData readFromFile(const std::string& inFilePath);
    bool writeToFile(const std::string& inFilePath, const RawImageData& rawData);

private:
    int m_bitsPerPixel = 8;
};

#endif // BMPLOADER_H
