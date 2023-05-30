#ifndef BMPCODER_H
#define BMPCODER_H

#include <string>
#include <vector>
#include <memory>

class RawImageData;
class EncodedImageData;
class DecodedImageData;

class BmpCoder {
public:
    BmpCoder()=default;
    ~BmpCoder();

    bool encode(const std::string& bmpFilePath, const std::string& barkFilePath);
    bool decode(const std::string& barkFilePath, const std::string& bmpFilePath);

private:
//    std::shared_ptr<RawImageData> m_rawImageData;
//    std::shared_ptr<EncodedImageData> m_encodedImageData;
//    std::shared_ptr<DecodedImageData> m_decodedImageData;

    std::vector<std::byte> encodeRow(const std::vector<std::byte>&) const;
    std::vector<std::byte> decodeRow(const std::vector<std::byte>&) const;
    void append(std::vector<std::byte>&, std::byte, int repeat = 1) const;

    bool load(const std::string& filePath);
    bool save(const std::string& filePath);

    bool encode(const std::shared_ptr<RawImageData>&);
    bool decode();

    friend class BmpCoderTest;
};

#endif // BMPCODER_H
