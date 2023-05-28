#ifndef BMPLOADER_H
#define BMPLOADER_H

#include <string>

#include "rawimagedata.h"

class BmpLoader {
public:
    BmpLoader()=default;
    ~BmpLoader()=default;

    bool readFromFile(const std::string& inFilePath);
    bool writeToFile(const std::string& inFilePath, const RawImageData& rawData);

    const RawImageData& data() const { return m_rawData; }

private:
    RawImageData m_rawData;
};

#endif // BMPLOADER_H
