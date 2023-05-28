#include "bmpcodertest.h"
#include <bmpcoder.h>

#include <QTest>

#include <iostream>

void BmpCoderTest::testCompression()
{
    BmpCoder coder;
    //QVERIFY(coder.compress("test-image-1-gs.bmp"));
    //QVERIFY(coder.compress("test-image-2-gs.bmp"));
}

void BmpCoderTest::testEncodingDecodingRow()
{
    BmpCoder coder;

    const std::vector<unsigned char> decoded1{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08}; // num % 4 = 0 case
    const std::vector<unsigned char> encoded1{0x00,0x02,0x03,0x01,0x02,0x04,0x08};

    const std::vector<unsigned char> decoded2{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08,0x09,0x11}; // num % 4 = 2 case
    const std::vector<unsigned char> encoded2{0x00,0x02,0x03,0x01,0x02,0x04,0x08,0x03,0x09,0x11};

    // test encoding
    QCOMPARE(coder.encodeRow(decoded1), encoded1);
    QCOMPARE(coder.encodeRow(decoded2), encoded2);

    // test decoding
    QCOMPARE(coder.decodeRow(encoded1), decoded1);
    QCOMPARE(coder.decodeRow(encoded2), decoded2);
}
