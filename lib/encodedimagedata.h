#ifndef ENCODEDIMAGEDATA_H
#define ENCODEDIMAGEDATA_H

#include "utils.h"

#include <cstdint>
#include <cstring>
#include <vector>

class EncodedImageData {
public:
    EncodedImageData()=default;
    EncodedImageData(std::size_t bytesNum, std::size_t rowsIndexesNum, unsigned int width, unsigned int height):
        m_width(width)
        , m_height(height)
    {
        m_bytes.resize(bytesNum);
        m_rowIndexes.resize(rowsIndexesNum);
    }
    EncodedImageData(unsigned int width, unsigned int height):
        m_width(width)
        , m_height(height)
    {}
    ~EncodedImageData()=default;

    bool isValid() const { return (m_width > 0) && (m_height > 0) && (m_rowIndexes.size() > 0) && (m_bytes.size() > 0) && (m_rowIndexes.size()%2==0); }

    unsigned int width() const { return m_width; }
    unsigned int height() const { return m_height; }

    const std::vector<std::byte>& rowIndexes() const { return m_rowIndexes; }
    const std::vector<std::byte>& bytes() const { return m_bytes; }

    void addEncodedRow(const std::vector<std::byte>& row) {
        uint16_t size = row.size();
        utils::extend(m_rowIndexes, getEncodedIndex(size));
        utils::extend(m_bytes, row);
    }

    static std::vector<std::byte> getEncodedIndex(uint16_t index) {
        std::vector<std::byte> bytes;
        bytes.resize(sizeof(uint16_t));
        // Copy the bytes of the uint16_t into the vector
        std::memcpy(bytes.data(), &index, sizeof(uint16_t));
        return std::move(bytes);
    }

    static uint16_t getDecodedIndex(const std::vector<std::byte>& bytes) {
        if (bytes.size() != 2) {
            return 0;
        }

        uint16_t result = 0;
        result |= static_cast<uint8_t>(bytes[0]); // Assign the first byte to the lower 8 bits
        result |= static_cast<uint16_t>(bytes[1]) << 8; // Assign the second byte to the upper 8 bits

        return result;
    }

private:
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    std::vector<std::byte> m_rowIndexes;
    std::vector<std::byte> m_bytes;

    friend class BarchLoader;
    friend class EncodedImageDataTest;
};

#endif // ENCODEDIMAGEDATA_H
