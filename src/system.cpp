#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// Done: Return the system's CPU
Processor& System::Cpu() {
  return cpu_;
}

// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    processes_.clear();

    vector<int> pids = LinuxParser::Pids();

  for(int processPid : pids)
  {
    std::string processUserName       = LinuxParser::User(processPid);
    std::string processCommand        = LinuxParser::Command(processPid);
    std::string processRam            = LinuxParser::Ram(processPid);
    long int processUpTime            = LinuxParser::UpTime(processPid) - LinuxParser::UpTime() ;
    float processCpuUtilization       = LinuxParser::ActiveJiffies(processPid)/ float(LinuxParser::ActiveJiffies());

    Process process(processPid, processUserName, processCommand, processCpuUtilization, processRam, processUpTime);

    processes_.push_back(process);
  }
  std::sort(processes_.begin(), processes_.end(), [](const Process& firstProcess, const Process& secondProcess){
      return secondProcess < firstProcess;
  });
  return processes_;
}

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Done: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Done: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Done: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Done: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
