#ifndef BMPCOMPRESSOR_H
#define BMPCOMPRESSOR_H

#include <string>

class BmpCompressor {
public:
    BmpCompressor()=default;
    ~BmpCompressor()=default;

    bool compress(const std::string& inFilePath) const;
};

#endif // BMPCOMPRESSOR_H
