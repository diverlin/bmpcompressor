#include "utils.h"

namespace utils {

int findNextDivisibleByFour(int num) {
    int remainder = num % 4;
    if (remainder == 0) {
        // The number is already divisible by 4
        return num;
    } else {
        // Calculate the next closest number divisible by 4
        return num + (4 - remainder);
    }
}

} // namespace utils
