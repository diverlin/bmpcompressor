#ifndef BMPCOMPRESSORTEST_H
#define BMPCOMPRESSORTEST_H

#include <QObject>

class BmpCoderTest : public QObject {
    Q_OBJECT
public:
    BmpCoderTest()=default;
    ~BmpCoderTest()=default;

private slots:
    void testCompression();
    void testEncodingDecodingRow();
};

#endif // BMPCOMPRESSORTEST_H
