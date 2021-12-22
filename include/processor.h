#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>
#include <map>
#include <numeric>

class Processor {
 public:
  float Utilization();
  void Update(std::vector<float> data);


 private:
  long prevIdle_{0};
  long prevTotal_{0};
  std::vector<float> data_;
};

#endif