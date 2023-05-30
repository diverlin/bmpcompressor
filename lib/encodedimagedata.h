#ifndef ENCODEDIMAGEDATA_H
#define ENCODEDIMAGEDATA_H

#include <cstdint>
#include <cstring>
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
        copy(getEncodedIndex(size), m_rowIndexes);
        copy(row, m_bytes);
    }

private:
    int m_width = 0;
    int m_height = 0;
    std::vector<std::byte> m_rowIndexes;
    std::vector<std::byte> m_bytes;

    std::vector<std::byte> getEncodedIndex(uint16_t index) {
        std::vector<std::byte> bytes;
        bytes.resize(sizeof(uint16_t));
        // Copy the bytes of the uint16_t into the vector
        std::memcpy(bytes.data(), &index, sizeof(uint16_t));
        return std::move(bytes);
    }

    uint16_t getDecodedIndex(const std::vector<std::byte>& bytes) {
        if (bytes.size() != 2) {
            return 0;
        }

        uint16_t result = 0;
        result |= static_cast<uint8_t>(bytes[0]); // Assign the first byte to the lower 8 bits
        result |= static_cast<uint16_t>(bytes[1]) << 8; // Assign the second byte to the upper 8 bits

        return result;
    }

    void copy(const std::vector<std::byte>& src, std::vector<std::byte>& dest) {
        std::size_t oldDestSize = dest.size();
        dest.resize(src.size()+dest.size());
        std::copy(src.begin(), src.end(), dest.begin()+oldDestSize);
    }

    friend class BarchLoader;
    friend class EncodedImageDataTest;
};

#endif // ENCODEDIMAGEDATA_H
