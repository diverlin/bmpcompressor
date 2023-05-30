#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QListView;
class QComboBox;

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

    MessageBox* m_messageBox = nullptr;

    void showMessageBox(const QString& title, const QString& message);

    void handleClickOnPng(const QString& fileName);
    void handleClickOnBmp(const QString& fileName);
    void handleClickOnBarch(const QString& fileName);
};

#endif // MAINWINDOW_H
