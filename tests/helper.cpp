#include "helper.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

std::vector<std::byte> toBytes(const std::vector<unsigned char>& in)
{
    std::vector<std::byte> bytes;
    for (unsigned char ch: in) {
        bytes.push_back(std::byte(ch));
    }
    return std::move(bytes);
};

std::string extractEmbedded(const QString& embeddedFilePath) {
    std::string result = "";
    QFile tmpFile(QFileInfo(embeddedFilePath).fileName());
    if (tmpFile.exists()) {
        tmpFile.remove();
    }

    if (tmpFile.open(QIODevice::WriteOnly)) {
        QFile resourceFile(embeddedFilePath);
        if (resourceFile.open(QIODevice::ReadOnly)) {
            tmpFile.write(resourceFile.readAll());
            result = tmpFile.fileName().toStdString();
        } else {
            qCritical() << "failed to open the embedded resource.";
        }
    } else {
        qCritical() << "failed to create a temporary file.";
    }

    return result;
}
