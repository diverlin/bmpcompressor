#include "bmpcodertest.h"
#include <bmpcoder.h>
#include <bmploader.h>
#include "helper.h"

#include <QTest>

#include <iostream>

void BmpCoderTest::testEncodeWhiteRow()
{
    BmpCoder coder(true);

    const std::vector<unsigned char> decoded1{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; // all white
    const std::vector<unsigned char> encoded1{};

    // test encoding
    QCOMPARE(coder.encodeRow(toBytes(decoded1), decoded1.size()), toBytes(encoded1));

    // test decoding
    QCOMPARE(coder.decodeRow(toBytes(encoded1), decoded1.size()), toBytes(decoded1));
}

void BmpCoderTest::testEncodeDecodeRow()
{
    BmpCoder coder;

    const std::vector<unsigned char> decoded1{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08}; // num % 4 = 0 case
    const std::vector<unsigned char> encoded1{0x00,0x02,0x03,0x01,0x02,0x04,0x08};

    const std::vector<unsigned char> decoded2{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08,0x09,0x11}; // num % 4 = 2 case
    const std::vector<unsigned char> encoded2{0x00,0x02,0x03,0x01,0x02,0x04,0x08,0x03,0x09,0x11};

    // test encoding
    QCOMPARE(coder.encodeRow(toBytes(decoded1), decoded1.size()), toBytes(encoded1));
    QCOMPARE(coder.encodeRow(toBytes(decoded2), decoded2.size()), toBytes(encoded2));

    // test decoding
    QCOMPARE(coder.decodeRow(toBytes(encoded1), decoded1.size()), toBytes(decoded1));
    QCOMPARE(coder.decodeRow(toBytes(encoded2), decoded2.size()), toBytes(decoded2));
}

void BmpCoderTest::testEncodeDecodeRows()
{
    BmpCoder coder;

    BmpLoader bpmLoader;
    auto rawData = bpmLoader.readFromFile(extractEmbedded(":/data/test-image-1-825x1200_gs.bmp"));

    auto encodedData = coder.encode(rawData);
    auto decodedData = coder.decode(encodedData);

    QCOMPARE(decodedData->width(), rawData->width());
    QCOMPARE(decodedData->height(), rawData->height());
    QCOMPARE(decodedData->bytes().size(), rawData->bytes().size());
    QCOMPARE(decodedData->bytes(), rawData->bytes());
}

void BmpCoderTest::testEncodeDecodeFile()
{
    QList<QString> patterns{"test-image-1-825x1200_gs", "test-image-2-825x1200_gs"};
    for (const QString& pattern: patterns) {
        {
            BmpCoder coder;
            QVERIFY(coder.encode(extractEmbedded(QString(":/data/%1.bmp").arg(pattern)), QString("%1.barch").arg(pattern).toStdString()));
            QVERIFY(coder.decode(QString("%1.barch").arg(pattern).toStdString(), QString("%1.decoded.bmp").arg(pattern).toStdString()));
            QVERIFY(coder.errorMsg().empty());
        }

        {
            BmpCoder coder(false);
            QVERIFY(coder.encode(extractEmbedded(QString(":/data/%1.bmp").arg(pattern)), QString("%1.nwcr.barch").arg(pattern).toStdString()));
            QVERIFY(coder.decode(QString("%1.nwcr.barch").arg(pattern).toStdString(), QString("%1.nwcr.decoded.bmp").arg(pattern).toStdString()));
            QVERIFY(coder.errorMsg().empty());
        }
    }
}
