#include "processor.h"


float Processor::Utilization()
{
  // Based on https://rosettacode.org/wiki/Linux_CPU_utilization
  // The resulting value is similar to the observed average usage over the cores

  auto total = std::accumulate(this->data_.begin(), this->data_.end(),
                               decltype(data_)::value_type(0));
  auto idle = this->data_[3];

  auto idle_ratio = (idle - prevIdle_) / (total  - prevTotal_);

  this->prevIdle_ = idle;
  this->prevTotal_ = total ;

  return  1.0 - idle_ratio;

}

void Processor::Update(std::vector<float> data)
{

  this->data_ = std::vector<float>(data.begin(), data.end()-3);
}
