#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "jobfactory.h"

class QListView;
class QComboBox;
class QLabel;

class FilesModel;
class MessageBox;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(const QString& filePath);

private:
    FilesModel* m_filesModel = nullptr;

    QListView* m_view = nullptr;
    QComboBox* m_cbExtFilter = nullptr;
    QWidget* m_centralWidget = nullptr;
    QLabel* m_lbLocation = nullptr;

    MessageBox* m_messageBox = nullptr;

    JobFactory m_jobFactory;

    void showMessageBox(const QString& title, const QString& message);

    void handleClickOnPng(const QString& fileName);
    void handleClickOnBmp(const QString& fileName);
    void handleClickOnBarch(const QString& fileName);

    QString resolveFilePath(QString path, QString filename) const;
};

#endif // MAINWINDOW_H
