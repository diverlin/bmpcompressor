#ifndef BMPWRITER_H
#define BMPWRITER_H

#include <string>

#include "rawimagedata.h"

class BmpWriter {
public:
    BmpWriter()=default;
    ~BmpWriter()=default;

    bool writeToFile(const std::string& inFilePath, const RawImageData& rawData);
};

#endif // BMPWRITER_H
