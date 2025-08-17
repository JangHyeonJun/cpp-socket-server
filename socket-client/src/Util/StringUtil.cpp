#include "util/StringUtil.h"

namespace util
{
    std::string CleanLine(std::istream& is) {
        std::string line;
        std::getline(is, line);

        while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
            line.pop_back();
        }
        return line;
    }
}
