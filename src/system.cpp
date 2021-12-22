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
    std::ifstream file;
    this->processes_.clear();
    auto pids = this->parser_.Pids();
    for (int pid : pids)
    {
    file.open("/proc/" + std::to_string(pid));
    if (file.is_open()) {
      Process P(pid);
      P.Update(parser_);
      this->processes_.push_back(P);
    }
    }
    sort(this->processes_.begin(), this->processes_.end());
    std::reverse(this->processes_.begin(), this->processes_.end());
}