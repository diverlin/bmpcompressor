#ifndef BMPLOADERTEST_H
#define BMPLOADERTEST_H

#include <QObject>

class BmpLoaderTest : public QObject {
    Q_OBJECT
public:
    BmpLoaderTest()=default;
    ~BmpLoaderTest()=default;

private slots:
    void testLoader();
};

#endif // BMPLOADERTEST_H
