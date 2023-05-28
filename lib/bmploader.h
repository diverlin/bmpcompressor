#ifndef BMPLOADER_H
#define BMPLOADER_H

#include <string>

#include "rawimagedata.h"

class BmpLoader {
public:
    BmpLoader()=default;
    ~BmpLoader()=default;

    bool load(const std::string& inFilePath);
    const RawImageData& data() const { return m_rawData; }

private:
    RawImageData m_rawData;
};

#endif // BMPLOADER_H
