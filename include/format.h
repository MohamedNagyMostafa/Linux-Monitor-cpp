#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // Done: See src/format.cpp
std::string DoubleDigit(std::string number);
};                                    // namespace Format

#endif