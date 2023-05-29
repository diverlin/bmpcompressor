#include <QCoreApplication>
#include <QTest>

#include "bmploadertest.h"
#include "bmpcodertest.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int testResult = 0;

    testResult |= QTest::qExec(new BmpLoaderTest, argc, argv);
    testResult |= QTest::qExec(new BmpCoderTest, argc, argv);

    return testResult;
}
