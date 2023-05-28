#ifndef BMPCODERTEST_H
#define BMPCODERTEST_H

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

#endif // BMPCODERTEST_H
