#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, std::string user, std::string command,
        float cpuUtilization, std::string ram, long int upTime)
    :_pid(pid), _user(user), _command(command), _cpuUtilization(cpuUtilization), _ram(ram), _upTime(upTime){};

// Done: Return this process's ID
int Process::Pid() const{
  return _pid;
}

// Done: Return this process's CPU utilization
float Process::CpuUtilization() const { return _cpuUtilization;}

// Done: Return the command that generated this process
string Process::Command() const { return _command; }

// Done: Return this process's memory utilization
string Process::Ram() const { return _ram; }

// Done: Return the user (name) that generated this process
string Process::User()  const{ return _user; }

// Done: Return the age of this process (in seconds)
long int Process::UpTime()  const { return _upTime; }


// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& k) const{

  return this->_cpuUtilization < k.CpuUtilization();
}