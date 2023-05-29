#include "bmploadertest.h"
#include <bmploader.h>

#include <QFileInfo>
#include <QFile>
#include <QTest>

#include <iostream>

namespace {

std::string extractEmbedded(const QString& embeddedFilePath) {
    std::string result = "";
    QFile tmpFile(QFileInfo(embeddedFilePath).fileName());
    if (tmpFile.exists()) {
        tmpFile.remove();
    }

    if (tmpFile.open(QIODevice::WriteOnly)) {
        QFile resourceFile(embeddedFilePath);
        if (resourceFile.open(QIODevice::ReadOnly)) {
            tmpFile.write(resourceFile.readAll());
            result = tmpFile.fileName().toStdString();
        } else {
            qCritical() << "failed to open the embedded resource.";
        }
    } else {
        qCritical() << "failed to create a temporary file.";
    }

    return result;
}

} // namespace


/* 2x1
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(0)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(1)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(2)=" 255
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(3)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(4)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(5)=" 255
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(6)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(7)=" 0
8 vs 6 */

/* 4x1
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(0)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(1)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(2)=" 255
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(3)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(4)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(5)=" 255
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(6)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(7)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(8)=" 255
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(9)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(10)=" 0
QINFO  : BmpLoaderTest::testReadWriteTiny24Bit() "byte(11)=" 0
12 vs 12 */

void BmpLoaderTest::testReadWriteTiny24Bit()
{
    BmpLoader loader(24);

    std::vector<QSize> sizes({QSize(1,1), QSize(2,1), QSize(3,1), QSize(4,1)});
    for (const QSize& size: sizes) {
        qInfo() << "process tiny size" << size;
        const int w = size.width();
        const int h = size.height();

        const QString embeddedInputFilePath = QString(":/data/%1x%2_24bits.bmp").arg(w).arg(h);
        const QString outputFilePath = "out_"+QFileInfo(embeddedInputFilePath).fileName();

        const RawImageData readedRawImageDataOrig = loader.readFromFile(extractEmbedded(embeddedInputFilePath));

        int counter=0;
        for (std::byte b: readedRawImageDataOrig.bytes()) {
            qInfo() << QString("byte(%1)=").arg(counter) << static_cast<unsigned char>(b);
            counter++;
        }

        QCOMPARE(readedRawImageDataOrig.width(), w);
        QCOMPARE(readedRawImageDataOrig.height(), h);
        QCOMPARE(readedRawImageDataOrig.bytes().size(), w*h*3);

        QVERIFY(loader.writeToFile(outputFilePath.toStdString(), readedRawImageDataOrig));

        const RawImageData readedRawImageDataTwin = loader.readFromFile(outputFilePath.toStdString());
        QCOMPARE(readedRawImageDataTwin.width(), w);
        QCOMPARE(readedRawImageDataTwin.height(), h);
        QCOMPARE(readedRawImageDataTwin.bytes(), readedRawImageDataOrig.bytes());
    }
}

//void BmpLoaderTest::testReadWrite24Bit()
//{
//    BmpLoader loader(24);

//    const QString embeddedInputFilePath = ":/data/128x64_24bits.bmp";
//    const QString outputFilePath = "out_"+QFileInfo(embeddedInputFilePath).fileName();

//    const RawImageData readedRawImageDataOrig = loader.readFromFile(extractEmbedded(embeddedInputFilePath));
//    QCOMPARE(readedRawImageDataOrig.width(), 128);
//    QCOMPARE(readedRawImageDataOrig.height(), 64);
//    //QCOMPARE(readedRawImageDataOrig.bytes().size(), 128*64*3);

//    QVERIFY(loader.writeToFile(outputFilePath.toStdString(), readedRawImageDataOrig));

//    const RawImageData readedRawImageDataTwin = loader.readFromFile(outputFilePath.toStdString());
//    QCOMPARE(readedRawImageDataTwin.width(), 128);
//    QCOMPARE(readedRawImageDataTwin.height(), 64);
//    //QCOMPARE(readedRawImageDataTwin.bytes(), readedRawImageDataOrig.bytes());
//}

//void BmpLoaderTest::testReadWrite8Bit()
//{
//    BmpLoader loader;

//    const QString embeddedInputFilePath = ":/data/test-image-1-825x1200_gs.bmp";
//    const QString outputFilePath = "out_"+QFileInfo(embeddedInputFilePath).fileName();

//    const RawImageData readedRawImageDataOrig = loader.readFromFile(extractEmbedded(embeddedInputFilePath));
//    QCOMPARE(readedRawImageDataOrig.width(), 825);
//    QCOMPARE(readedRawImageDataOrig.height(), 1200);
//    //QCOMPARE(readedRawImageDataOrig.bytes().size(), 825*1200);

//    QVERIFY(loader.writeToFile(outputFilePath.toStdString(), readedRawImageDataOrig));

//    const RawImageData readedRawImageDataTwin = loader.readFromFile(outputFilePath.toStdString());
//    QCOMPARE(readedRawImageDataTwin.width(), 825);
//    QCOMPARE(readedRawImageDataTwin.height(), 1200);
//    //QCOMPARE(readedRawImageDataTwin.bytes(), readedRawImageDataOrig.bytes());
//}
