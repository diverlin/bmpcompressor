#include "mainwindow.h"
#include "filemodel.h"

#include <QListView>
#include <QFileInfo>
#include <QVBoxLayout>

MainWindow::MainWindow(const QString& startupPath)
    : QMainWindow()
    , m_view(new QListView(this))
{
    qInfo() << "use" << startupPath << "as startup location";

    m_filesModel = new FilesModel(startupPath, {"*.png","*.bmp","*.barch"});

    m_view->setModel(m_filesModel);

    m_view->show();

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    QVBoxLayout* layout = new QVBoxLayout;
    m_centralWidget->setLayout(layout);

    layout->addWidget(m_view);
}


