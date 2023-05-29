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

int findNextDivisibleByFour(int num) {
    int remainder = num % 4;
    if (remainder == 0) {
        // The number is already divisible by 4
        return num;
    } else {
        // Calculate the next closest number divisible by 4
        return num + (4 - remainder);
    }
}

} // namespace

void BmpLoaderTest::testReadWriteTiny24Bit()
{
    const int bitsPerPixel = 24;
    const int bytesPerPixel = bitsPerPixel / 8;
    BmpLoader loader(bitsPerPixel);

    std::vector<QSize> sizes({QSize(1,1), QSize(2,1), QSize(3,1), QSize(4,1)});
    for (const QSize& size: sizes) {
        qInfo() << "process tiny size" << size;
        const int w = size.width();
        const int h = size.height();

        const QString embeddedInputFilePath = QString(":/data/%1x%2_24bits.bmp").arg(w).arg(h);
        const QString outputFilePath = "out_"+QFileInfo(embeddedInputFilePath).fileName();

        const RawImageData readedRawImageDataOrig = loader.readFromFile(extractEmbedded(embeddedInputFilePath));

//        int counter=0;
//        for (std::byte b: readedRawImageDataOrig.bytes()) {
//            qInfo() << QString("byte(%1)=").arg(counter) << static_cast<unsigned char>(b);
//            counter++;
//        }

        QCOMPARE(readedRawImageDataOrig.width(), w);
        QCOMPARE(readedRawImageDataOrig.height(), h);
        QCOMPARE(readedRawImageDataOrig.bytes().size(), findNextDivisibleByFour(bytesPerPixel*w)*h);

        QVERIFY(loader.writeToFile(outputFilePath.toStdString(), readedRawImageDataOrig));

        const RawImageData readedRawImageDataTwin = loader.readFromFile(outputFilePath.toStdString());
        QCOMPARE(readedRawImageDataTwin.width(), w);
        QCOMPARE(readedRawImageDataTwin.height(), h);
        QCOMPARE(readedRawImageDataTwin.bytes(), readedRawImageDataOrig.bytes());
    }
}

void BmpLoaderTest::testReadWrite24Bit()
{
    const int bitsPerPixel = 24;
    const int bytesPerPixel = bitsPerPixel / 8;
    BmpLoader loader(bitsPerPixel);

    const QString embeddedInputFilePath = ":/data/128x64_24bits.bmp";
    const QString outputFilePath = "out_"+QFileInfo(embeddedInputFilePath).fileName();

    const RawImageData readedRawImageDataOrig = loader.readFromFile(extractEmbedded(embeddedInputFilePath));
    QCOMPARE(readedRawImageDataOrig.width(), 128);
    QCOMPARE(readedRawImageDataOrig.height(), 64);
    QCOMPARE(readedRawImageDataOrig.bytes().size(), findNextDivisibleByFour(bytesPerPixel*128)*64);

    QVERIFY(loader.writeToFile(outputFilePath.toStdString(), readedRawImageDataOrig));

    const RawImageData readedRawImageDataTwin = loader.readFromFile(outputFilePath.toStdString());
    QCOMPARE(readedRawImageDataTwin.width(), 128);
    QCOMPARE(readedRawImageDataTwin.height(), 64);
    QCOMPARE(readedRawImageDataTwin.bytes(), readedRawImageDataOrig.bytes());
}

void BmpLoaderTest::testReadWrite8Bit()
{
    const int bytesPerPixel = 1;
    BmpLoader loader;

    const QString embeddedInputFilePath = ":/data/test-image-1-825x1200_gs.bmp";
    const QString outputFilePath = "out_"+QFileInfo(embeddedInputFilePath).fileName();

    const RawImageData readedRawImageDataOrig = loader.readFromFile(extractEmbedded(embeddedInputFilePath));
    QCOMPARE(readedRawImageDataOrig.width(), 825);
    QCOMPARE(readedRawImageDataOrig.height(), 1200);
    const int padding = 3;
    QCOMPARE(readedRawImageDataOrig.bytes().size(), findNextDivisibleByFour(bytesPerPixel*825)*1200);

    QVERIFY(loader.writeToFile(outputFilePath.toStdString(), readedRawImageDataOrig));

    const RawImageData readedRawImageDataTwin = loader.readFromFile(outputFilePath.toStdString());
    QCOMPARE(readedRawImageDataTwin.width(), 825);
    QCOMPARE(readedRawImageDataTwin.height(), 1200);
    QCOMPARE(readedRawImageDataTwin.bytes(), readedRawImageDataOrig.bytes());
}
