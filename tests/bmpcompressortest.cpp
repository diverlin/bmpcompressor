#include "bmpcompressortest.h"
#include <bmpcompressor.h>

#include <QTest>

#include <iostream>

void BmpCompressorTest::testCompression()
{
    BmpCompressor compressor;
    QVERIFY(compressor.compress("test-image-1-gs.bmp"));
    QVERIFY(compressor.compress("test-image-2-gs.bmp"));
}

void BmpCompressorTest::testEncodingDecodingRow()
{
    BmpCompressor encoder;

    const std::vector<unsigned char> decoded1{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08}; // num % 4 = 0 case
    const std::vector<unsigned char> encoded1{0x00,0x02,0x03,0x01,0x02,0x04,0x08};

    const std::vector<unsigned char> decoded2{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08,0x09,0x11}; // num % 4 = 2 case
    const std::vector<unsigned char> encoded2{0x00,0x02,0x03,0x01,0x02,0x04,0x08,0x03,0x09,0x11};

    // test encoding
    QCOMPARE(encoder.encodeRow(decoded1), encoded1);
    QCOMPARE(encoder.encodeRow(decoded2), encoded2);

    // test decoding
    QCOMPARE(encoder.decodeRow(encoded1), decoded1);
    QCOMPARE(encoder.decodeRow(encoded2), decoded2);
}
