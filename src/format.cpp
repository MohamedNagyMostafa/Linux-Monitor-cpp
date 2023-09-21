#include <string>

#include "format.h"

using std::string;

// Done: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long totalSeconds) {
  int hoursNum, minutesNum, secondsNum;

  hoursNum   = int(totalSeconds/3600);
  minutesNum = int(((totalSeconds / 3600.) - hoursNum)  * 60);

  secondsNum = int(((((totalSeconds / 3600.) - hoursNum) * 60 ) - minutesNum) * 60);

  return DoubleDigit(std::to_string(hoursNum)) + ":" + DoubleDigit(std::to_string(minutesNum)) + ":" + DoubleDigit(std::to_string(secondsNum));
}

string Format::DoubleDigit(std::string number)
{
  return (number.length() < 2)? "0" + number : number;
}