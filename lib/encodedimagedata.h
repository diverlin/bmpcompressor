#ifndef ENCODEDIMAGEDATA_H
#define ENCODEDIMAGEDATA_H

#include <cstdint>
#include <vector>

class EncodedImageData {
public:
    EncodedImageData()=default;
    EncodedImageData(unsigned int width, unsigned int height):
        m_width(width)
        , m_height(height)
    {}
    ~EncodedImageData()=default;

    int width() const { return m_width; }
    int height() const { return m_height; }

    const std::vector<std::byte>& rowIndexes() const { return m_rowIndexes; }
    const std::vector<std::byte>& bytes() const { return m_bytes; }

    void addEncodedRow(const std::vector<std::byte>& row) {
        uint16_t size = row.size();
        copy(uint16ToBytes(size), m_rowIndexes);
        copy(row, m_bytes);
    }

private:
    int m_width = 0;
    int m_height = 0;
    std::vector<std::byte> m_rowIndexes;
    std::vector<std::byte> m_bytes;

    std::vector<std::byte> uint16ToBytes(uint16_t value) {
        std::vector<std::byte> bytes;
        bytes.resize(sizeof(uint16_t));

        // Copy the bytes of the uint16_t into the vector
        std::memcpy(bytes.data(), &value, sizeof(uint16_t));

        return bytes;
    }

    void copy(const std::vector<std::byte>& src, std::vector<std::byte>& dest) {
        std::copy(src.begin(), src.end(), dest.begin());
    }

    friend class BarchLoader;
};

#endif // ENCODEDIMAGEDATA_H
