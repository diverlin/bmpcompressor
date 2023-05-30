#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(const QString& filePath);

private:
    QWidget* m_centralWidget = nullptr;
};

#endif // MAINWINDOW_H
