#ifndef BMPCOMPRESSORTEST_H
#define BMPCOMPRESSORTEST_H

#include <QObject>

class BmpCompressorTest : public QObject {
    Q_OBJECT
public:
    BmpCompressorTest()=default;
    ~BmpCompressorTest()=default;

private slots:
//    void testCompression();
    void testEncodingDecodingRow();
};

#endif // BMPCOMPRESSORTEST_H
