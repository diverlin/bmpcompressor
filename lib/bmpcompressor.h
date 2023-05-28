#ifndef BMPCOMPRESSOR_H
#define BMPCOMPRESSOR_H

#include <string>
#include <vector>

class BmpCompressor {
public:
    BmpCompressor()=default;
    ~BmpCompressor()=default;

    bool compress(const std::string& inFilePath) const;

private:
    std::string encodeRow(const std::string&) const;
    std::string decodeRow(const std::string&) const;

    friend class BmpCompressorTest;
};

#endif // BMPCOMPRESSOR_H
