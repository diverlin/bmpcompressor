#include "mainwindow.h"
#include "filesmodel.h"
#include "messagebox.h"
#include "bmpcoder.h"

#include <QListView>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>

MainWindow::MainWindow(const QString& startupPath)
    : QMainWindow()
    , m_view(new QListView(this))
    , m_messageBox(new MessageBox)
    , m_jobFactory(this)
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

    m_lbLocation = new QLabel(this);
    m_lbLocation->setText(startupPath);

    layout->addWidget(m_cbExtFilter);
    layout->addWidget(m_view);
    layout->addWidget(m_lbLocation);

    connect(m_cbExtFilter, &QComboBox::currentTextChanged, this, [this](const QString& currentText){
        qInfo() << "current text changed=" << currentText;
        m_filesModel->adjustFilter(currentText);
    });

    connect(m_view, &QListView::clicked, [this](const QModelIndex &index) {
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


    connect(&m_jobFactory, &JobFactory::jobDone, this, [this](const QString& cmd, const QString& input, const QString& output, const QString& error){
        if (!error.isEmpty()) {
            showMessageBox("Job error", error);
        }
        m_filesModel->handleJobFinished(QFileInfo(input).fileName());
    });
}

void MainWindow::handleClickOnPng(const QString& fileName)
{
    showMessageBox("wrong format", "The *.png format is not supported!");
}

QString MainWindow::resolveFilePath(QString path, QString fileName) const
{
    if (path.isEmpty()) {
        return fileName;
    } else {
        return path + "/" + fileName;
    }
}

void MainWindow::handleClickOnBmp(const QString& fileName)
{
    QString cmd = "encode";
    QString input = resolveFilePath(m_filesModel->rootPath(), fileName);
    QString baseFileName = QString(fileName).replace(".bmp", "");
    QString output = resolveFilePath(m_filesModel->rootPath(), baseFileName + ".packed.barch");

    m_jobFactory.pushJob(cmd, input, output);
    m_filesModel->handleJobStarted(cmd, QFileInfo(input).fileName());
}

void MainWindow::handleClickOnBarch(const QString& fileName)
{
    QString cmd = "decode";
    QString input = resolveFilePath(m_filesModel->rootPath(), fileName);
    QString baseFileName = QString(fileName).replace(".packed.barch", "");
    QString output = resolveFilePath(m_filesModel->rootPath(), baseFileName + ".unpacked.bmp");

    m_jobFactory.pushJob(cmd, input, output);
    m_filesModel->handleJobStarted(cmd, QFileInfo(input).fileName());
}

void MainWindow::showMessageBox(const QString& title, const QString& message)
{
    m_messageBox->setMessage(title, message);
    m_messageBox->show();
    m_messageBox->raise();
}

