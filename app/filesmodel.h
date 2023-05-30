#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QTimer>

#include <QAbstractTableModel>

class FilesModel final : public QAbstractTableModel
{
public:
    explicit FilesModel(const QString& rootPath, const QList<QString>& extensionFilters, QObject* parent = nullptr);
    ~FilesModel() override final;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override final;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override final;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override final;

private:
    struct FileItem {
        QString name;
        QString size;
    };

    QString m_rootPath;
    QList<FileItem> m_fileData;
    QList<QString> m_extensionFilters;

    void refresh();

    QTimer m_refreshTimer;
};

#endif // FILEMODEL_H
