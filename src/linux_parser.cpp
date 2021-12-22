#include "linux_parser.h"

#include "format.h"
using std::string;

/* ---------------------------------------------------------- */

// File read operations

LinuxParser::LinuxParser()
{
    ReadProcMeminfo();
    ReadProcStat();
    ReadProcUptime();
    ReadEtcPasswd();
}

void LinuxParser::UpdateData()
{
  ReadProcMeminfo();
  ReadProcStat();
  ReadProcUptime();
  ReadEtcPasswd();
}

void LinuxParser::ReadProcStat()
{
  std::string temp;
  this->file_.open(kProcDirectory + kStatFilename);
  assert(this->file_.is_open() == true);
  this->procStatData_.clear();
  while(std::getline(file_,temp))
  {
    this->procStatData_.push_back(temp);
  }
  this->file_.close();
}

void LinuxParser::ReadEtcPasswd()
{
  std::string temp;
  this->file_.open(kPasswordPath);
  assert(this->file_.is_open() == true);
  this->etcPasswdData_.clear();
  while(std::getline(file_,temp))
  {
    this->etcPasswdData_.push_back(ParseUserData(temp));
  }
  this->file_.close();
}

void LinuxParser::ReadProcUptime()
{
  std::string temp;
  this->file_.open(kProcDirectory + kUptimeFilename);
  assert(this->file_.is_open() == true);
  std::getline(this->file_, temp);
  this->procUptimeData_ = temp;
  this->file_.close();
}

void LinuxParser::ReadProcMeminfo()
{
  // https://support.site24x7.com/portal/en/kb/articles/how-is-memory-utilization-calculated-for-a-linux-server-monitor
  // based on the article we only need the first 3 lines for ubuntu based systems
  // The resulting memory utilization matches that of ubuntu system monitor.

  std::string temp;
  int nFetchLines = 3, i = 0;

  this->file_.open(kProcDirectory + kMeminfoFilename);
  assert(this->file_.is_open() == true);
  this->procMemData_.clear();
  while(std::getline(file_,temp) && i < nFetchLines)
  {
    this->procMemData_.push_back(temp);
    ++i;
  }
  this->file_.close();
}

void LinuxParser::ReadProcessStat(int Pid)
{
  std::string temp, temp2;
  this->file_.open(kProcDirectory + std::to_string(Pid) + kStatFilename);
  assert(this->file_.is_open() == true);
  std::getline(this->file_, temp);
  std::istringstream ss(temp);
  this->processStatData_.clear();
  while (!ss.eof())
  {
    ss >> temp2;
    this->processStatData_.push_back(temp2);
  }
  this->file_.close();

}

void LinuxParser::ReadProcessStatus(int Pid)
{
  std::string temp;
  std::vector<std::string> tempVec;
  std::string filename = kProcDirectory + std::to_string(Pid) + kStatusFilename;

  this->file_.open(filename);
  assert(this->file_.is_open() == true);
  while (std::getline(file_, temp)) {
    tempVec.push_back(temp);
  }
  this->file_.close();
  // Takes only the lines with UID and VMSize
  this->processStatusData_.clear();
  this->processStatusData_ = {tempVec[8], tempVec[17]};
}

void LinuxParser::ReadProcessCmdline(int pid)
{
  std::string temp;
  this->file_.open(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  assert(this->file_.is_open() == true);
  std::getline(this->file_, temp);
  this->processCmdlineData_ = temp;
  this->file_.close();
}

/*--------------------------------------------------------------*/

// Process

int LinuxParser::GetProcessUid()
{
  std::string s1,s2;
  std::istringstream ss(this->processStatusData_[0]);
  // Uid:	1000	1000	1000	1000
  ss >> s1 >> s2;
  return stoi(s2);
}

long LinuxParser::GetProcessRam()
{

  std::string s1,s2;
  std::istringstream ss(this->processStatusData_[1]);
  // VmSize:	  246736 kB
  ss >> s1 >> s2;
  return stoi(s2);
}

// BONUS: Update Pids() to use std::filesystem
std::vector<int> LinuxParser::Pids() {
  std::vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

long LinuxParser::GetProcessUptime()
{
  return  std::stol(this->processStatData_[21]) / sysconf(_SC_CLK_TCK);
}

std::vector<long> LinuxParser::GetProcessTimes()
{
  std::vector<long> data = {
              std::stol(this->processStatData_[13]),
              std::stol(this->processStatData_[14]),
              std::stol(this->processStatData_[15]),
              std::stol(this->processStatData_[16]),
              std::stol(this->processStatData_[21])};

  return   data;
}

std::string LinuxParser::GetProcessUsername()
{
  auto uid = this->GetProcessUid();
  for(auto u : this->etcPasswdData_)
  {
    if(std::stoi(u[2]) == uid)
    {
      return u[0];
    }
  }
  return  "none";
}

std::string  LinuxParser::GetProcessCommandLine()
{
  std::istringstream ss(this->processCmdlineData_);
  ss >> this->processCmdlineData_;
  return  this->processCmdlineData_;
}

std::vector<std::string> LinuxParser::ParseUserData(std::string &user)
{
  std::vector<std::string> udata;
  std::istringstream s(user);
  std::string token;

  while(std::getline(s, token, ':'))
  {
    udata.push_back(token);
  }

  return udata;
}

/* -------------------------------------------------------------- */

// System

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

long int LinuxParser::UpTime()
{
  long int tmp;
  std::istringstream s(this->procUptimeData_);
  s >> tmp;
  return tmp;
}

int LinuxParser::TotalProcesses()
{
  int tp_count;
  std::string name;
  std::istringstream s(this->procStatData_[this->procStatData_.size() - 4]);
  s >> name >> tp_count;
  return tp_count;
}

int LinuxParser::RunningProcesses()
{
  int rp_count;
  std::string name;
  std::istringstream s(this->procStatData_[this->procStatData_.size() - 3]);
  s >> name >> rp_count;
  return rp_count;
}

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

float LinuxParser::MemoryUtilization()
{
  std::istringstream s;
  std::string t1;
  std::vector<float> memdata{5};
  int i = 0;
  for(auto &d : this->procMemData_)
  {
    s.str(d);
    s >> t1 >> memdata[i++];
    s.clear();
  }

  auto total = memdata[0];
  auto free = memdata[2];
  return (total - free)/total;
}

std::vector<float> LinuxParser::CpuUtilization()
{
  std::vector<float> cpu_data;
  std::string name;
  float tmp;
  std::istringstream s(this->procStatData_[0]);
  s >> name;
  while(!s.eof())
  {
    s >> tmp;
    cpu_data.push_back(tmp);
  }
  return  cpu_data;
}

/* ---------------------------------------------------------------- */
