#ifndef BMPCOMPRESSOR_H
#define BMPCOMPRESSOR_H

#include <string>
#include <vector>

class BmpCoder {
public:
    BmpCoder()=default;
    ~BmpCoder()=default;

    bool compress(const std::string& inFilePath) const;

private:
    std::vector<unsigned char> encodeRow(const std::vector<unsigned char>&) const;
    std::vector<unsigned char> decodeRow(const std::vector<unsigned char>&) const;
    void append(std::vector<unsigned char>&, unsigned char, int repeat = 1) const;

    friend class BmpCoderTest;
};

#endif // BMPCOMPRESSOR_H
