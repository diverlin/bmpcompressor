#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>

namespace Ui {
    class MessageBox;
}

class MessageBox : public QWidget
{
Q_OBJECT
public:
    MessageBox();
    void setMessage(const QString& title, const QString& message);

private:
    Ui::MessageBox* m_ui = nullptr;

};

#endif // MAINWINDOW_H
