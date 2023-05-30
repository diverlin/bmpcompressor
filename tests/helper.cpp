#include "helper.h"

std::vector<std::byte> toBytes(const std::vector<unsigned char>& in)
{
    std::vector<std::byte> bytes;
    for (unsigned char ch: in) {
        bytes.push_back(std::byte(ch));
    }
    return std::move(bytes);
};
