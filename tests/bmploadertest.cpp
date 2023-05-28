#include "bmploadertest.h"
#include <bmploader.h>

#include <QFileInfo>
#include <QFile>

#include <QTest>

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

void BmpLoaderTest::testReader()
{
    BmpLoader loader;
    QVERIFY(loader.readFromFile(extractEmbedded(":/data/test-image-1-gs.bmp")));
    QCOMPARE(loader.data().width, 825);
    QCOMPARE(loader.data().height, 1200);

    QVERIFY(loader.readFromFile(extractEmbedded(":/data/test-image-2-gs.bmp")));
    QCOMPARE(loader.data().width, 825);
    QCOMPARE(loader.data().height, 1200);
}

void BmpLoaderTest::testWriter()
{
    BmpLoader loader;
    QVERIFY(loader.readFromFile(extractEmbedded(":/data/test-image-1-gs.bmp")));
    QVERIFY(loader.writeToFile("sample.bmp", loader.data()));
}
