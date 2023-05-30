#ifndef BMPLOADER_H
#define BMPLOADER_H

#include <string>
#include <memory>

#include "rawimagedata.h"

class BmpLoader {
public:
    BmpLoader(int bitsPerPixel = 8);
    ~BmpLoader()=default;

    std::shared_ptr<RawImageData> readFromFile(const std::string& filePath);
    bool writeToFile(const std::string& filePath, const RawImageData& rawData);

    std::string errorMsg() const { return m_errorMsg; }

private:
    int m_bitsPerPixel = 8;
    std::string m_errorMsg;

    void error(const std::string& msg);
};

#endif // BMPLOADER_H
