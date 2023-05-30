#include "jobfactory.h"
#include <bmpcoder.h>

#include <QThread>
#include <QFileInfo>
#include <QDebug>


Worker::Worker(const QString& cmd, const QString& input, const QString& output)
    : m_cmd(cmd)
    , m_input(input)
    , m_output(output)
{

}

Worker::~Worker()
{
    qInfo() << "thread=" << QThread::currentThreadId() << "~Worker for"<< m_cmd << m_input << m_output;
}

QString Worker::error() const { return m_error; }

void Worker::doWork()
{
    if (m_cmd == "encode") {
        BmpCoder coder;
        if (!coder.encode(m_input.toStdString(), m_output.toStdString())) {
            m_error = coder.errorMsg().c_str();
        }
    } else if (m_cmd == "decode") {
        BmpCoder coder;
        if (!coder.decode(m_input.toStdString(), m_output.toStdString())) {
            m_error = coder.errorMsg().c_str();
        }
    } else {
        m_error = "unknown job cmd requested";
    }
    emit workFinished(m_cmd, m_input, m_output, m_error);
    deleteLater();
}



JobFactory::JobFactory(QObject* parent, bool multithreading)
    : QObject(parent)
    , m_multithreading(multithreading)
{

}

JobFactory::~JobFactory()
{
}

void JobFactory::pushJob(const QString& cmd, const QString& input, const QString& output)
{
    if (m_multithreading) {
        Worker* worker = new Worker(cmd, input, output);
        QThread* thread = new QThread;
        worker->moveToThread(thread);

        // Connect signals and slots between the worker and the thread
        connect(thread, &QThread::started, worker, &Worker::doWork);
        connect(worker, &Worker::workFinished, this, &JobFactory::jobDone);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);

        thread->start();
    } else {
        qWarning() << "JobFactory is running in single thread mode, this is slow and used for tests";
        Worker worker(cmd, input, output);
        worker.doWork();
        QString error = worker.error();

        emit jobDone(cmd, input, output, error);
    }
}




