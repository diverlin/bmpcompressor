#include "bmpcodertest.h"
#include <bmpcoder.h>
#include "helper.h"

#include <QTest>

#include <iostream>

void BmpCoderTest::testEncodeDecodeFile()
{
    BmpCoder coder;
    //QVERIFY(coder.encode(extractEmbedded(":/data/test-image-1-825x1200_gs.bmp"), "test-image-1-825x1200_gs.barch"));
    //QVERIFY(coder.decode("test-image-1-825x1200_gs.barch", "_decoded_test-image-1-825x1200_gs.bmp"));
}

void BmpCoderTest::testEncodeDecodeRow()
{
    BmpCoder coder;

    const std::vector<unsigned char> decoded1{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08}; // num % 4 = 0 case
    const std::vector<unsigned char> encoded1{0x00,0x02,0x03,0x01,0x02,0x04,0x08};

    const std::vector<unsigned char> decoded2{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08,0x09,0x11}; // num % 4 = 2 case
    const std::vector<unsigned char> encoded2{0x00,0x02,0x03,0x01,0x02,0x04,0x08,0x03,0x09,0x11};

    // test encoding
    QCOMPARE(coder.encodeRow(toBytes(decoded1)), toBytes(encoded1));
    QCOMPARE(coder.encodeRow(toBytes(decoded2)), toBytes(encoded2));

    // test decoding
    QCOMPARE(coder.decodeRow(toBytes(encoded1)), toBytes(decoded1));
    QCOMPARE(coder.decodeRow(toBytes(encoded2)), toBytes(decoded2));
}
