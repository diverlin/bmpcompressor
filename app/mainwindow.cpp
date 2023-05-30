#include "mainwindow.h"
#include "filesmodel.h"
#include "messagebox.h"
#include "bmpcoder.h"

#include <QListView>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QComboBox>

MainWindow::MainWindow(const QString& startupPath)
    : QMainWindow()
    , m_view(new QListView(this))
    , m_messageBox(new MessageBox)
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
        if (fileName.toLower().endsWith(".png")) {
            handleClickOnPng(fileName);
        } else if (fileName.toLower().endsWith(".bmp")) {
            handleClickOnBmp(fileName);
        } else if (fileName.toLower().endsWith(".barch")) {
            handleClickOnBarch(fileName);
        }
    });
}

void MainWindow::handleClickOnPng(const QString& fileName)
{
    showMessageBox("wrong format", "The *.png format is not supported!");
}

void MainWindow::handleClickOnBmp(const QString& fileName)
{
    const std::string input = QString(m_filesModel->rootPath()+"/"+fileName).toStdString();
    QString baseFileName = QString(fileName).replace(".bmp", "");
    std::string output = QString(m_filesModel->rootPath()+"/"+baseFileName + ".packed.barch").toStdString();

    BmpCoder coder;

    if (!coder.encode(input, output)) {
        showMessageBox("encoding error", coder.errorMsg().c_str());
    }
}

void MainWindow::handleClickOnBarch(const QString& fileName)
{
    const std::string input = QString(m_filesModel->rootPath()+"/"+fileName).toStdString();
    QString baseFileName = QString(fileName).replace(".packed.barch", "");
    std::string output = QString(m_filesModel->rootPath()+"/"+baseFileName + ".unpacked.bmp").toStdString();

    BmpCoder coder;

    if (!coder.decode(input, output)) {
        showMessageBox("decoding error", coder.errorMsg().c_str());
    }
}

void MainWindow::showMessageBox(const QString& title, const QString& message)
{
    m_messageBox->setMessage(title, message);
    m_messageBox->show();
    m_messageBox->raise();
}

