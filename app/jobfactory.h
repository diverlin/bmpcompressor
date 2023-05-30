#ifndef JOBFACTORY_H
#define JOBFACTORY_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(const QString& cmd, const QString& input, const QString& output);
    ~Worker();

    QString error() const;

public slots:
    void doWork();

signals:
    void workFinished(QString, QString, QString, QString);

private:
    QString m_cmd;
    QString m_input;
    QString m_output;
    QString m_error;
};


class JobFactory : public QObject
{
    Q_OBJECT

public:
    JobFactory(QObject* parent, bool multithreading = true);
    ~JobFactory();

    void pushJob(const QString& cmd, const QString& input, const QString& output);

signals:
    void jobDone(const QString& cmd, const QString& input, const QString& output, const QString& error);

private:
    bool m_multithreading = true;
};

#endif // JOBFACTORY_H
