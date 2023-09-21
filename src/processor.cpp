#include "processor.h"
#include <unistd.h>

// Done: Return the aggregate CPU utilization
float Processor::Utilization() {
  long activeJiffies = LinuxParser::ActiveJiffies();
  long idleJiffies  = LinuxParser::IdleJiffies();

  long activeJiffiesChange  = activeJiffies - _prevActiveJifffies;
  long idleJiffiesChange    = idleJiffies - _prevIdleJiffies;

  long totalUtilization = activeJiffiesChange + idleJiffiesChange;

  return activeJiffiesChange / float(totalUtilization);
}


