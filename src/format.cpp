#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds)
{
  int HH = seconds / 3600;
  int MM = (seconds % 3600 ) / 60;
  int SS = seconds % 60 ;

  string formatted_time = std::to_string(HH) + ":" + std::to_string(MM) + ":" + std::to_string(SS);
  return  formatted_time;
}