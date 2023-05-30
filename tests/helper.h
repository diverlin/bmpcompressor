#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <string>

#include <QString>

std::vector<std::byte> toBytes(const std::vector<unsigned char>& in);

std::string extractEmbedded(const QString& embeddedFilePath);

#endif // HELPER_H
