#ifndef BARCHREADERTEST_H
#define BARCHREADERTEST_H

#include <QObject>

class BarchLoaderTest : public QObject {
    Q_OBJECT
public:
    BarchLoaderTest()=default;
    ~BarchLoaderTest()=default;

private slots:
    void testReadWrite();
};

#endif // BARCHREADERTEST_H
