#include "bmpcompressortest.h"
#include <bmpcompressor.h>

#include <QTest>

void BmpCompressorTest::testCompression()
{
    BmpCompressor compressor;
    QVERIFY(compressor.compress("test-image-1-gs.bmp"));
    QVERIFY(compressor.compress("test-image-2-gs.bmp"));
}
