#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QListView;

class FilesModel;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(const QString& filePath);

private:
    FilesModel* m_filesModel = nullptr;
    QListView* m_view = nullptr;
    QWidget* m_centralWidget = nullptr;
};

#endif // MAINWINDOW_H
