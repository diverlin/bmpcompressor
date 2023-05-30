#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

namespace utils {

int findNextDivisibleByFour(int num);

void extend(std::vector<std::byte>& body, const std::vector<std::byte>& suffix);

bool endsWith(const std::string& fullString, const std::string& ending);

} // namespace utils

#endif // UTILS_H
