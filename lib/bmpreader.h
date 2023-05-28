#ifndef BMPREADER_H
#define BMPREADER_H

#include <string>

#include "rawimagedata.h"

class BmpReader {
public:
    BmpReader()=default;
    ~BmpReader()=default;

    bool readFromFile(const std::string& inFilePath);
    const RawImageData& data() const { return m_rawData; }

private:
    RawImageData m_rawData;
};

#endif // BMPREADER_H
