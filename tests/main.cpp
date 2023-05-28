#include <QCoreApplication>
#include <QTest>

#include "bmploadertest.h"
#include "bmpcompressortest.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int testResult = 0;

    testResult |= QTest::qExec(new BmpLoaderTest, argc, argv);
    //testResult |= QTest::qExec(new BmpCompressorTest, argc, argv);

    return testResult;
}
