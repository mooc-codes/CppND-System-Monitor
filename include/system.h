#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization() const;
  long UpTime() const;
  int TotalProcesses() const;
  int RunningProcesses() const;
  std::string Kernel();
  std::string OperatingSystem();
  void Update();

 private:
  LinuxParser parser_;
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  int total_processes_;
  int running_processes_;
  long int uptime_;
  float memory_utilization_;

  // constant
  std::string os_;
  std::string kernel_;

  void FindProcesses();

};

#endif