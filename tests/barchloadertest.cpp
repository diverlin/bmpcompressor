#include "barchloadertest.h"
#include <barchloader.h>

#include "helper.h"

#include <QTest>

void BarchLoaderTest::testReadWrite()
{
    EncodedImageData encodedData(800, 600);

    const std::vector<std::byte> row1 = toBytes({0x01,0x02,0x03});
    const std::vector<std::byte> row2 = toBytes({0x11,0x05});
    const std::vector<std::byte> row3 = toBytes({0x06});
    const std::vector<std::byte> row4 = toBytes({0x04,0x12});

    encodedData.addEncodedRow(row1);
    encodedData.addEncodedRow(row2);
    encodedData.addEncodedRow(row3);
    encodedData.addEncodedRow(row4);

    BarchLoader loader;
    loader.writeToFile("test.barch", encodedData);

    //EncodedImageData loadedEncodedData = loader.readFromFile("test.barch");

//    QCOMPARE(encodedData.width(), loadedEncodedData.width());
//    QCOMPARE(encodedData.height(), loadedEncodedData.height());
//    QCOMPARE(encodedData.bytes(), loadedEncodedData.bytes());
//    QCOMPARE(encodedData.rowIndexes(), loadedEncodedData.rowIndexes());
}
