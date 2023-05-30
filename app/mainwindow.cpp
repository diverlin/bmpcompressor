#include "mainwindow.h"

#include <QFileInfo>

MainWindow::MainWindow(const QString& startupPath)
    : QMainWindow()
{
    qInfo() << "use" << startupPath << "as startup location";

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
}


