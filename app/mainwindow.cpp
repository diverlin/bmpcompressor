#include "mainwindow.h"
#include "filesmodel.h"

#include <QListView>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QComboBox>

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

    m_cbExtFilter = new QComboBox;
    m_cbExtFilter->addItem(m_filesModel->extensionFilters().join(", "));
    for (const QString& filter: qAsConst(m_filesModel->extensionFilters())) {
        m_cbExtFilter->addItem(filter);
    }

    layout->addWidget(m_cbExtFilter);
    layout->addWidget(m_view);

    connect(m_cbExtFilter, &QComboBox::currentTextChanged, this, [this](const QString& currentText){
        qInfo() << "current text changed=" << currentText;
        m_filesModel->adjustFilter(currentText);
    });

    QObject::connect(m_view, &QListView::clicked, [this](const QModelIndex &index) {
        QString fileName = m_filesModel->data(index, FilesModel::FileNameDataRole).toString();
        qDebug() << "Clicked file:" << fileName;
    });
}


