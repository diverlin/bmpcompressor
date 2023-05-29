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

    auto toBytes = [](const std::vector<unsigned char>& in) {
        std::vector<std::byte> bytes;
        for (unsigned char ch: in) {
            bytes.push_back(std::byte(ch));
        }
        return std::move(bytes);
    };

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
