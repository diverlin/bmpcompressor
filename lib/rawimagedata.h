#ifndef RAWIMAGEDATA_H
#define RAWIMAGEDATA_H

#include "utils.h"

#include <cstdint>
#include <vector>

class RawImageData {
public:
    RawImageData()=default;
    RawImageData(unsigned int width, unsigned int height)
        :
        m_width(width)
        , m_height(height) {}
    RawImageData(unsigned int width, unsigned int height, unsigned int expectedBytesNum)
        :
        m_width(width)
        , m_height(height)
    {
        m_bytes.resize(expectedBytesNum);
    }
    ~RawImageData()=default;

    void addDecodedRow(const std::vector<std::byte>& row) {
        utils::extend(m_bytes, row);
    }

    bool isValid() const { return (m_width > 0) && (m_height > 0) && (m_bytes.size() > 0); }
    int width() const { return m_width; }
    int height() const { return m_height; }

    const std::vector<std::byte>& bytes() const { return m_bytes; }

private:
    int m_width = 0;
    int m_height = 0;

    std::vector<std::byte> m_bytes;

    friend class BmpLoader;
};

#endif // RAWIMAGEDATA_H
