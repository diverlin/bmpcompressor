#include "bmpcompressortest.h"
#include <bmpcompressor.h>

#include <QTest>

void BmpCompressorTest::testCompression()
{
    BmpCompressor compressor;
    QVERIFY(compressor.compress());
}
