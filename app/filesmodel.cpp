#include "filesmodel.h"

#include <QDir>

FilesModel::FilesModel(const QString& rootPath, const QList<QString>& extensionFilters, QObject *parent)
    : QAbstractTableModel(parent)
    , m_rootPath(rootPath)
    , m_extensionFilters(extensionFilters)
{
    m_refreshTimer.setInterval(3000);
    refresh();

    connect(&m_refreshTimer, &QTimer::timeout, this, &FilesModel::refresh);
    m_refreshTimer.start();
}

FilesModel::~FilesModel()
{

}

void FilesModel::adjustFilter(const QString& filters)
{
    m_extensionFilters = filters.split(", ");
    refresh();
}

void FilesModel::refresh()
{
    m_fileData.clear();

    QDir directory(m_rootPath);

    directory.setNameFilters(m_extensionFilters);
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot); // Set filters for files and exclude "." and ".."

    QFileInfoList fileList = directory.entryInfoList();
    for (const QFileInfo& fi: fileList) {
        int fileSizeInKb = fi.size()/100;
        m_fileData.append(FileItem{fi.fileName(), QString("%1Kb").arg(fileSizeInKb)});
    }

    // Emit signals to notify the view of the data changes
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(rowCount() - 1, 0);
    emit dataChanged(topLeft, bottomRight);
}

int FilesModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_fileData.count();
}

int FilesModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 2; // Number of custom fields
}

QVariant FilesModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role) {
    case Qt::DisplayRole: {
        const FileItem& item = m_fileData.at(index.row());
        return QString("%1 (%2)").arg(item.name).arg(item.size);
    }
    case FileNameDataRole: {
        const FileItem& item = m_fileData.at(index.row());
        return item.name;
    }
    }

    return QVariant();
}

QVariant FilesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "Name";
        case 1:
            return "Size";
        }
    }

    return QVariant();
}




