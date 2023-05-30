#include "barchloadertest.h"
#include <barchloader.h>

//#include <QFileInfo>
//#include <QFile>
#include <QTest>

//#include <iostream>

void BarchLoaderTest::testReadWrite()
{
//    BarchLoader loader;

//    const QString embeddedInputFilePath = ":/data/128x64_24bits.bmp";
//    const QString outputFilePath = "out_"+QFileInfo(embeddedInputFilePath).fileName();

//    const EncodedImageData readedRawImageDataOrig = loader.readFromFile(extractEmbedded(embeddedInputFilePath));
//    QCOMPARE(readedRawImageDataOrig.width(), 128);
//    QCOMPARE(readedRawImageDataOrig.height(), 64);
//    QCOMPARE(readedRawImageDataOrig.bytes().size(), findNextDivisibleByFour(bytesPerPixel*128)*64);

//    QVERIFY(loader.writeToFile(outputFilePath.toStdString(), readedRawImageDataOrig));

//    const RawImageData readedRawImageDataTwin = loader.readFromFile(outputFilePath.toStdString());
//    QCOMPARE(readedRawImageDataTwin.width(), 128);
//    QCOMPARE(readedRawImageDataTwin.height(), 64);
//    QCOMPARE(readedRawImageDataTwin.bytes(), readedRawImageDataOrig.bytes());
}
