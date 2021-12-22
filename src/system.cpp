#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

long int System::UpTime() const { return this->uptime_; }

std::string System::OperatingSystem() { return this->os_; }

int System::RunningProcesses() const { return this->running_processes_; }

int System::TotalProcesses() const { return this->total_processes_; }

std::string System::Kernel() { return this->kernel_; }

float System::MemoryUtilization() const { return this->memory_utilization_; }

Processor& System::Cpu() { return this->cpu_; }

vector<Process>& System::Processes() { return processes_; }


System::System(): parser_(LinuxParser()), cpu_(Processor())
{
  this->os_ = this->parser_.OperatingSystem();
  this->kernel_ = this->parser_.Kernel();
  this->memory_utilization_ = this->parser_.MemoryUtilization();
  this->total_processes_ = this->parser_.TotalProcesses();
  this->running_processes_ = this->parser_.RunningProcesses();
  this->uptime_ = this->parser_.UpTime();
  this->cpu_.Update(this->parser_.CpuUtilization());
  this->FindProcesses();
}

void System::Update()
{
  this->parser_.UpdateData();
  this->memory_utilization_ = this->parser_.MemoryUtilization();
  this->total_processes_ = this->parser_.TotalProcesses();
  this->running_processes_ = this->parser_.RunningProcesses();
  this->uptime_ = this->parser_.UpTime();
  this->cpu_.Update(this->parser_.CpuUtilization());
  this->FindProcesses();
}

void System::FindProcesses()
{

  this->processes_.clear();
  auto pids = this->parser_.Pids();
  for (int pid : pids)
  {
    if (std::filesystem::exists("/proc/" + std::to_string(pid))) {
      Process P(pid);
      P.Update(parser_);
      this->processes_.push_back(P);
    }
  }
  sort(this->processes_.begin(), this->processes_.end());
  std::reverse(this->processes_.begin(), this->processes_.end());
}