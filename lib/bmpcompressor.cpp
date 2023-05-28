#include "bmpcompressor.h"
#include "bmploader.h"

bool BmpCompressor::compress(const std::string& inFilePath) const
{
    bool result = false;

    BmpLoader loader;
    if (loader.load(inFilePath)) {
        const RawImageData& data = loader.data();
        // todo implement processing
        result = false;
    }

    return result;
}

