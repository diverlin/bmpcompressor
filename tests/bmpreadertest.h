#ifndef BMPREADERTEST_H
#define BMPREADERTEST_H

#include <QObject>

class BmpLoaderTest : public QObject {
    Q_OBJECT
public:
    BmpLoaderTest()=default;
    ~BmpLoaderTest()=default;

private slots:
    void testLoader();
};

#endif // BMPREADERTEST_H
