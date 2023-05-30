#ifndef BMPCODER_H
#define BMPCODER_H

#include <string>
#include <vector>
#include <memory>

class RawImageData;
class EncodedImageData;

class BmpCoder {
public:
    BmpCoder()=default;
    ~BmpCoder();

    bool encode(const std::string& bmpFilePath, const std::string& barkFilePath);
    bool decode(const std::string& barkFilePath, const std::string& bmpFilePath);

    const std::string errorMsg() const { return m_errorMsg; }

private:
    std::string m_errorMsg;
    std::vector<std::byte> encodeRow(const std::vector<std::byte>&) const;
    std::vector<std::byte> decodeRow(const std::vector<std::byte>&) const;
    void append(std::vector<std::byte>&, std::byte, int repeat = 1) const;

    std::shared_ptr<RawImageData> loadBmp(const std::string& filePath);
    std::shared_ptr<EncodedImageData> loadBarch(const std::string& filePath);

    bool save(const std::string& filePath, const std::shared_ptr<RawImageData>& data);
    bool save(const std::string& filePath, const std::shared_ptr<EncodedImageData>& data);

    std::shared_ptr<EncodedImageData> encode(const std::shared_ptr<RawImageData>&);
    std::shared_ptr<RawImageData> decode(const std::shared_ptr<EncodedImageData>&);

    void error(const std::string&);

    friend class BmpCoderTest;
};

#endif // BMPCODER_H
