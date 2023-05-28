#include "bmpcompressortest.h"
#include <bmpcompressor.h>

#include <QTest>

#include <iostream>

//void BmpCompressorTest::testCompression()
//{
//    BmpCompressor compressor;
//    QVERIFY(compressor.compress("test-image-1-gs.bmp"));
//    QVERIFY(compressor.compress("test-image-2-gs.bmp"));
//}

namespace {

std::string stringify(const std::vector<int>& arr) {
    std::string result;
    result.reserve(arr.size());

    for (int e: arr) {
        result += static_cast<char>(e);
    }
    return std::move(result);
}

} // namespace


void BmpCompressorTest::testEncodingDecodingRow()
{
    BmpCompressor encoder;

    const std::string decoded1 = stringify({0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01});
    const std::string encoded1 = stringify({0x00,0x02,0x03,0x01,0x01,0x01,0x01}); // "0101100000001000000010000000100000001"

    const std::string decoded2 = stringify({0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08});
    const std::string encoded2 = stringify({0x00,0x02,0x03,0x01,0x02,0x04,0x08}); // "0101100000001000000100000010000001000"

    // test encoding
    QCOMPARE(encoder.encodeRow(decoded1), encoded1);
    QCOMPARE(encoder.encodeRow(decoded2), encoded2);

    // test decoding
    QCOMPARE(encoder.decodeRow(encoded1), decoded1);
    QCOMPARE(encoder.decodeRow(encoded2), decoded2);
}
