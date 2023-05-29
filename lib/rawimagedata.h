#ifndef RAWIMAGEDATA_H
#define RAWIMAGEDATA_H

#include <cstdint>
#include <vector>

class RawImageData {
public:
    RawImageData()=default;
    RawImageData(unsigned int width, unsigned int height, unsigned int expectedBytesNum):
        m_width(width)
        , m_height(height)
        , m_expectedBytesNum(expectedBytesNum)
    {
        m_bytes.resize(expectedBytesNum);
    }
    ~RawImageData()=default;

    int width() const { return m_width; }
    int height() const { return m_height; }

    const std::vector<std::byte>& bytes() const { return m_bytes; }

private:
    int m_width = 0;
    int m_height = 0;
    std::vector<std::byte> m_bytes;
    std::size_t m_expectedBytesNum = 0;

    friend class BmpLoader;
};

#endif // RAWIMAGEDATA_H
