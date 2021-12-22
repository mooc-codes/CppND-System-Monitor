#include <unistd.h>
#include <string>
#include <vector>

#include "process.h"


using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) :pid_(pid){}

int Process::Pid() const { return this->pid_; }

float Process::CpuUtilization() const
{
        return this->cpu_utilization_;
}

string Process::Command() { return this->cmd_; }

string Process::Ram() { return this->ram_; }

string Process::User() { return this->username_; }

long int Process::UpTime() const { return this->uptime_; }

bool Process::operator<(Process const& a) const
{ return this->cpu_utilization_ < a.cpu_utilization_; }

void Process::ComputeCpuUtilization(LinuxParser &parser)
{
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat

  float Hertz = sysconf(_SC_CLK_TCK);
  float uptime = parser.UpTime();
  auto stat = parser.GetProcessTimes();
  float total_time = stat[0] + stat[2] + stat[1] + stat[3];
  float starttime = stat[4];

  float seconds = uptime - (starttime / Hertz);
  this->cpu_utilization_ = ((total_time / Hertz) / seconds);

}

void Process::Update(LinuxParser &parser)
{
  parser.ReadProcessStatus(this->pid_);
  parser.ReadProcessStat(this->pid_);
  parser.ReadProcessCmdline(this->pid_);

  this->ram_ = std::to_string(parser.GetProcessRam() /  1024);
  this->username_ = parser.GetProcessUsername();
  this->uptime_ = parser.GetProcessUptime();
  this->cmd_ = parser.GetProcessCommandLine();
  this->ComputeCpuUtilization(parser);

}

