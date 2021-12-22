#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds)
{
  int HH = seconds / 3600;
  int MM = (seconds % 3600 ) / 60;
  int SS = seconds % 60 ;

  string formatted_time = PadZero(HH) + ":" + PadZero(MM) + ":" + PadZero(SS);
  return  formatted_time;
}

string Format::PadZero(int unit)
{
    std::ostringstream ss;
    ss << std::setw( 2 ) << std::setfill( '0' ) << unit;
    return ss.str();
}