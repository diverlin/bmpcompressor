#include "messagebox.h"
#include "ui_messagebox.h"

#include <QTimer>

MessageBox::MessageBox()
:
    QWidget()
    , m_ui(new Ui::MessageBox)
{
    m_ui->setupUi(this);

    setWindowTitle("");
    setWindowFlags(Qt::FramelessWindowHint);

    connect(m_ui->bnAccept, &QPushButton::clicked, this, [this](){
        setStyleSheet("background-color: lightblue;");
        QTimer::singleShot(500, [this](){
            hide();
            setStyleSheet("background-color: white;");
        });
    });
}

void MessageBox::setMessage(const QString& title, const QString& message)
{
    m_ui->lbTitle->setText(title);
    m_ui->lbMessage->setText(message);
}
