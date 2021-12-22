#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  explicit Process(int pid);
  int Pid() const ;
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime() const;
  bool operator<(Process const& a) const;
  void Update(LinuxParser &parser);       // TODO: Implement in derived


 private:
  int pid_;
  std::string  username_;
  float cpu_utilization_{0};
  std::string ram_{0};
  long int uptime_{0};
  std::string cmd_{"none"};

  void ComputeCpuUtilization(LinuxParser &parser);
};


#endif