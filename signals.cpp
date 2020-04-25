#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    string error = "smash: got ctrl-Z";
    if (write(STDOUT_FILENO, error.c_str(), error.length()) != error.length()){
        perror("smash error: write failed");
    }
    SmallShell& smash = SmallShell::getInstance();
    kill(smash.current_fg_pid, SIGSTOP);
    JobsList::JobEntry* jobEntry = smash.jobsList.getJobByPid(smash.current_fg_pid);
    if(jobEntry != nullptr) {
        jobEntry->time_added = time(nullptr);
        jobEntry->stopped = true;
    }
    else{
        smash.addJob(smash.getCmdLine(), smash.current_fg_pid, true);
    }
}

void ctrlCHandler(int sig_num) {
    kill(SmallShell::getInstance().current_fg_pid, SIGKILL);
  // TODO: Add your implementation
}

void alarmHandler(int sig_num) {
  // TODO: Add your implementation
}

