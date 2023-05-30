#include <QApplication>

#include <mainwindow.h>
#include <filesmodel.h>

#include <QFileInfo>
#include <QFileSystemModel>
#include <QListView>


int main(int argc, char *argv[])
{
    QString startupPath;
    if (argc >= 2) {
        startupPath = argv[1];
    }

    QFileInfo fi(startupPath);
    if (fi.exists()) {
        if (fi.isFile()) {
            qInfo() << "startupPath" << startupPath << "is file, take path";
            startupPath = fi.filePath();
        }
    } else {
        qInfo() << "startupPath" << startupPath << "doesn't exists";
        startupPath = "";
    }

    if (startupPath.isEmpty()) {
        startupPath = QCoreApplication::applicationDirPath();
    }

    QApplication app(argc, argv);

    MainWindow* window = new MainWindow(startupPath);
    window->resize(800, 600);
    window->show();

    return app.exec();
}
