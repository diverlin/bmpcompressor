#ifndef BMPCODER_H
#define BMPCODER_H

#include <string>
#include <vector>

class BmpCoder {
public:
    BmpCoder()=default;
    ~BmpCoder()=default;

    bool compress(const std::string& inFilePath) const;

private:
    std::vector<std::byte> encodeRow(const std::vector<std::byte>&) const;
    std::vector<std::byte> decodeRow(const std::vector<std::byte>&) const;
    void append(std::vector<std::byte>&, std::byte, int repeat = 1) const;

    friend class BmpCoderTest;
};

#endif // BMPCODER_H
