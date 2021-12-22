#ifndef MONITOR_LINUX_PARSER_H
#define MONITOR_LINUX_PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include <string>
#include <vector>
#include <numeric>

#include <regex>
#include <cassert>
#include <dirent.h>


class LinuxParser
{
 private:
  // Paths
  const std::string kProcDirectory{"/proc/"};
  const std::string kCmdlineFilename{"/cmdline"};
//  const std::string kCpuinfoFilename{"/cpuinfo"};
  const std::string kStatusFilename{"/status"};
  const std::string kStatFilename{"/stat"};
  const std::string kUptimeFilename{"/uptime"};
  const std::string kMeminfoFilename{"/meminfo"};
  const std::string kVersionFilename{"/version"};
  const std::string kOSPath{"/etc/os-release"};
  const std::string kPasswordPath{"/etc/passwd"};

  std::ifstream  file_;

  std::string procUptimeData_;
  std::vector<std::string> procMemData_;
  std::vector<std::string> procStatData_;
  std::vector<std::vector<std::string>> etcPasswdData_;

  // Single storage reused for storing data temporarily for each process
  std::vector<std::string> processStatusData_;
  std::vector<std::string> processStatData_;
  std::string  processCmdlineData_;


  void ReadProcStat();
  void ReadProcMeminfo();
  void ReadProcUptime();
  void ReadEtcPasswd();

  static std::vector<std::string> ParseUserData(std::string &user);

 public:

  LinuxParser();
  void UpdateData();


  // System Data
  std::string OperatingSystem();
  std::string Kernel();
  int TotalProcesses();
  int RunningProcesses();
  long int UpTime();
  float MemoryUtilization();
  std::vector<float> CpuUtilization();

  // Process Data
  std::vector<int> Pids();
  void ReadProcessStatus(int Pid);
  void ReadProcessStat(int Pid);
  void ReadProcessCmdline(int pid);
  long GetProcessRam();
  int GetProcessUid();
  std::string GetProcessUsername();
  long GetProcessUptime();
  std::string  GetProcessCommandLine();
  std::vector<long>  GetProcessTimes();
};

#endif  // MONITOR_LINUX_PARSER_H
