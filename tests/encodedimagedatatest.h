#ifndef ENCODEDIMAGEDATATEST_H
#define ENCODEDIMAGEDATATEST_H

#include <QObject>

class EncodedImageDataTest : public QObject {
    Q_OBJECT
public:
    EncodedImageDataTest()=default;
    ~EncodedImageDataTest()=default;

private slots:
    void testEncodeDecodeIndex();
    void testConstruct();
};

#endif // ENCODEDIMAGEDATATEST_H
