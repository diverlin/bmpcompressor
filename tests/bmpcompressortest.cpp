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
    for (int e: arr) {
        std::cout << static_cast<char>(e) << std::endl;
        result += static_cast<char>(e);
    }
    return std::move(result);
}

} // namespace


void BmpCompressorTest::testEncodingRow()
{
    BmpCompressor encoder;

    std::cout << "str=" << stringify({255, 255, 0, 0}) << std::endl;
    QCOMPARE(encoder.encodeRow(stringify({0xff, 0xff, 0xff, 0xff,
                                          0x00, 0x00, 0x00, 0x00,
                                          0x01, 0x01, 0x01, 0x01})), "0101100000001000000010000000100000001");
    QCOMPARE(encoder.encodeRow(stringify({0xff, 0xff, 0xff, 0xff,
                                          0x00, 0x00, 0x00, 0x00,
                                          0x01, 0x02, 0x04, 0x08})), "0101100000001000000100000010000001000");
}
