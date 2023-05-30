#ifndef BMPCODERTEST_H
#define BMPCODERTEST_H

#include <QObject>

class BmpCoderTest : public QObject {
    Q_OBJECT
public:
    BmpCoderTest()=default;
    ~BmpCoderTest()=default;

private slots:
    void testEncodeDecodeFile();
    void testEncodeDecodeRow();
};

#endif // BMPCODERTEST_H
