#ifndef RAWIMAGEDATA_H
#define RAWIMAGEDATA_H

#include <cstdint>

struct RawImageData {
    RawImageData()=default;
    ~RawImageData() {
        clear();
    }
    void clear() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
        dataSize = 0;
        width = -1;
        height = -1;
    }
    uint64_t size() const { return width*height; }

    bool hasData() const { return data != nullptr; }
    int width = -1;
    int height = -1;
    unsigned char* data = nullptr;
    uint64_t dataSize = 0;
};

#endif // RAWIMAGEDATA_H
