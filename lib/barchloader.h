#ifndef BARCHLOADER_H
#define BARCHLOADER_H

#include <string>
#include <memory>

#include "encodedimagedata.h"

class BarchLoader {
public:
    BarchLoader()=default;
    ~BarchLoader()=default;

    std::shared_ptr<EncodedImageData> readFromFile(const std::string& filePath);
    bool writeToFile(const std::string& filePath, const EncodedImageData& encodedData);
};

#endif // BARCHLOADER_H
