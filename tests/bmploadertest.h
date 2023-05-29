#ifndef BMPREADERTEST_H
#define BMPREADERTEST_H

#include <QObject>

class BmpLoaderTest : public QObject {
    Q_OBJECT
public:
    BmpLoaderTest()=default;
    ~BmpLoaderTest()=default;

private slots:
    void testReadWriteTiny24Bit();
    void testReadWrite24Bit();
//    void testReadWrite8Bit();
};

#endif // BMPREADERTEST_H
