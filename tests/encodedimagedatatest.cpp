#include "encodedimagedatatest.h"
#include <encodedimagedata.h>

#include "helper.h"

#include <QTest>

void EncodedImageDataTest::testEncodeDecodeIndex()
{
    EncodedImageData data;

    QCOMPARE(data.getEncodedIndex(9), toBytes({0x09, 0x00}));
    QCOMPARE(data.getDecodedIndex(toBytes({0x09, 0x00})), 9);
}

void EncodedImageDataTest::testConstruct()
{
    EncodedImageData data;

    const std::vector<std::byte> row1 = toBytes({0x01,0x02,0x03});
    const std::vector<std::byte> row2 = toBytes({0x11,0x05});
    const std::vector<std::byte> row3 = toBytes({0x06});
    const std::vector<std::byte> row4 = toBytes({0x04,0x12});

    data.addEncodedRow(row1);
    data.addEncodedRow(row2);
    data.addEncodedRow(row3);
    data.addEncodedRow(row4);

    QCOMPARE(data.bytes(), toBytes({0x01,0x02,0x03,0x11,0x05,0x06,0x04,0x12}));
    QCOMPARE(data.rowIndexes(), toBytes({0x03,0x00,0x02,0x00,0x01,0x00,0x02,0x00}));
}
