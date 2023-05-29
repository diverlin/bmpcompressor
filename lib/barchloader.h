#ifndef BARCHLOADER_H
#define BARCHLOADER_H

#include <string>

#include "encodedimagedata.h"

class BarchLoader {
public:
    BarchLoader()=default;
    ~BarchLoader()=default;

    EncodedImageData readFromFile(const std::string& filePath);
    bool writeToFile(const std::string& filePath, const EncodedImageData& encodedData);
};

#endif // BMPLOADER_H
