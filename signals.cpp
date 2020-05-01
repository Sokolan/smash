#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"


#define write_with_error(error) \
if (write(STDOUT_FILENO, error.c_str(), error.length()) != error.length()) {\
perror("smash error: write failed");\
}

using namespace std;

void ctrlZHandler(int sig_num) {
    string error = "smash: got ctrl-Z";
    error += "\n";
    write_with_error(error);


    SmallShell& smash = SmallShell::getInstance();

    smash.fg_is_pipe ? killpg(smash.current_fg_pid, SIGSTOP) : kill(smash.current_fg_pid, SIGSTOP);

    error = "smash: process ";
    error += to_string(SmallShell::getInstance().current_fg_pid);
    error += " was stopped\n";
    write_with_error(error);


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
    string error = "smash: got ctrl-C\n";
    write_with_error(error);
    kill(SmallShell::getInstance().current_fg_pid, SIGKILL);
  // TODO: Add your implementation
}

void alarmHandler(int sig_num) {
    string message = "smash: got an alarm\n";
    write_with_error(message);

    SmallShell& smash = SmallShell::getInstance();
    TimeOut::TimeEntry* timeEntry = smash.timeOut.firstTimeEntry();
    message = "smash: timout ";
    message += to_string(timeEntry->duration) += timeEntry->cmd_line_command += " timed out!";
    write_with_error(message);

    if(timeEntry->pid != smash.getPid()){
        kill(timeEntry->pid, SIGKILL);
    }

    smash.timeOut.removeFirstTimeEntry();
    if((timeEntry = smash.timeOut.firstTimeEntry()) != nullptr){
        time_t curr_time = time(nullptr);
        alarm(timeEntry->duration - (curr_time - timeEntry->timestamp));
    }

}

