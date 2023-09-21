#include "linux_parser.h"
#include <bits/stdc++.h>
#include <dirent.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
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

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
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

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  long usedMemory;
  long totalMemory;

  std::ifstream fileStream(kProcDirectory + kMeminfoFilename);

  if(fileStream.is_open())
  {
    string line, key;
    long  value;

    while( std::getline(fileStream, line))
    {
      std::istringstream lineStream(line);

      lineStream >>key ;
      lineStream >> value;

      if(key == "MemFree:")
      {
        usedMemory = totalMemory - value;
        break;
      }
       totalMemory = value;
    }
  }

  return  usedMemory/float(totalMemory);
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  long totalUpTime = 0;

  std::ifstream fileStream(kProcDirectory + kUptimeFilename);

  if(fileStream.is_open())
  {
    long suspendTime;
    string line, ignore;

    std::getline(fileStream, line);
    std::istringstream stringStream(line);

    stringStream >> suspendTime;

    totalUpTime =  suspendTime ;
  }
  return totalUpTime;
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {

  long total_capacity = 0;

  for(string value : CpuUtilization())
  {
    total_capacity += std::stol(value);
  }
  return  total_capacity;
}

// Done: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  float cpuUtilization  = 0;

  std::ifstream fileStream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);

  if(fileStream.is_open())
  {
    std::string value;
    std::vector<std::string> cpuUtilizationParams;

    while(std::getline(fileStream, value, ' '))
    {
       cpuUtilizationParams.push_back(value);
    }

    long uTime  = std::stol(cpuUtilizationParams[14]);
    long sTime  = std::stol(cpuUtilizationParams[15]);

    cpuUtilization  = uTime + sTime;

    return cpuUtilization / sysconf(_SC_CLK_TCK);
  }
  return cpuUtilization;
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {

  std::vector<std::string> cpuUsageParameters = CpuUtilization();

  long long cpuUser  = std::stol(cpuUsageParameters[kUser_]);
  long long cpuNice  = std::stol(cpuUsageParameters[kNice_]);
  long long cpuSystem  = std::stol(cpuUsageParameters[kSystem_]);
  long long cpuIrq     = std::stol(cpuUsageParameters[kIRQ_]);
  long long cpuSoftirq = std::stol(cpuUsageParameters[kSoftIRQ_]);
  long long cpuSteal   = std::stol(cpuUsageParameters[kSteal_]);
  long long cpuGuest   = std::stol(cpuUsageParameters[kGuest_]);
  long long cpuGuestNice   = std::stol(cpuUsageParameters[kGuestNice_]);
  return (cpuUser + cpuNice + cpuSystem + cpuIrq + cpuSoftirq + cpuGuest + cpuGuestNice + cpuSteal);
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<std::string> cpuUsageParameters = CpuUtilization();
  long long cpuIdle     = std::stol(cpuUsageParameters[kIdle_]);
  long long cpuIowait    = std::stol(cpuUsageParameters[kIOwait_]);

  return cpuIdle + cpuIowait;
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> utilizationData;

  std::ifstream fileStream(kProcDirectory + kStatFilename);

  if(fileStream.is_open())
  {
    string line, next;
    std::getline(fileStream, line);
    std::istringstream lineStream(line);


    while(lineStream >> next)
    {
      if(next != "cpu")
        utilizationData.push_back(next);
    }

  }
  return utilizationData;
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int processes = 0;
  string wordPicker;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if(stream.is_open())
  {
    string line;
    while(std::getline(stream, line))
    {
      std::istringstream lineStream(line);
      lineStream >> wordPicker;
      if(wordPicker == "processes")
      {
        lineStream >> processes;
        break;
      }
    }
  }
  return processes;
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcessors = 0;
  string wordPicker;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if(stream.is_open())
  {
    string line;
    while(std::getline(stream, line))
    {
      std::istringstream lineStream(line);
      lineStream >> wordPicker;
      if(wordPicker == "procs_running")
      {
        lineStream >> runningProcessors;
        break;
      }
    }
  }
  return runningProcessors;
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string commandLine;

  std::ifstream fileStream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if(fileStream.is_open())
  {
    std::getline(fileStream, commandLine);
  }
  return commandLine;
}

// Done: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  long utilizedMemory;

  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatusFilename);

  if(fileStream.is_open())
  {
    string line, key;

    while(std::getline(fileStream, line))
    {
      std::istringstream stringStream(line);

      stringStream >> key >> utilizedMemory;

      if(key == "VmSize:") break;
    }
  }
  return  to_string(int(utilizedMemory/1000.));
}

// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string userUid;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatusFilename);

  if(fileStream.is_open())
  {
    string line;
    while(std::getline(fileStream, line))
    {
      string key, value;

      std::istringstream lineStream(line);
      lineStream >> key >> value;

      if(key == "Uid:")
      {
        userUid = value;
        break;
      }
    }
  }

  return userUid;
}

// Done: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string userName, userUid;

  // Find UID of the process.
  userUid = Uid(pid);

  // Find username
  std::ifstream fileStream(kPasswordPath);

  if(fileStream.is_open())
  {
    string line, ignore, currUid;
    while(std::getline(fileStream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), '-', '_');

      std::istringstream stringStream(line);

      stringStream >> userName >> ignore >> currUid;

      if(currUid == userUid) break;
    }
  }

  return userName;
}

// Done: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long upTime = 0;

  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatFilename);

  if(fileStream.is_open())
  {
    string value;
    int counter = 0;
    while(std::getline(fileStream, value, ' '))
    {
      if(counter == 22)
      {
        upTime = std::stol(value);
        break;
      }
      counter++;
    }

  }

  return upTime/ sysconf(_SC_CLK_TCK);
}
