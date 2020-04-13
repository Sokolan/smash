#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <list>

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)
using namespace std;


class Command {
// TODO: Add your data members
    string name;
    const char* cmd_line_command;
    char** args;
    int num_of_args;
 public:
    explicit Command(const char* cmd_line);
    char* get_arg(int i) const;
    virtual ~Command();
    virtual void execute() = 0;
    //virtual void prepare();
    //virtual void cleanup();
    // TODO: Add your extra methods if needed
};

class BuiltInCommand : public Command {
 public:
    explicit BuiltInCommand(const char* cmd_line);
    virtual ~BuiltInCommand() {};
};

class ExternalCommand : public Command {
 public:
  ExternalCommand(const char* cmd_line);
    virtual ~ExternalCommand() {}
    void execute() override;
};

class PipeCommand : public Command {
  // TODO: Add your data members
 public:
    PipeCommand(const char* cmd_line);
    virtual ~PipeCommand() {}
    void execute() override;
};

class RedirectionCommand : public Command {
 // TODO: Add your data members
 public:
    explicit RedirectionCommand(const char* cmd_line);
    virtual ~RedirectionCommand() {}
    void execute() override;
    //void prepare() override;
    //void cleanup() override;
};

class ChangeDirCommand : public BuiltInCommand {
// TODO: Add your data members
    string lastPwd;
public:
    explicit ChangeDirCommand(const char* cmd_line, string plastPwd);
    virtual ~ChangeDirCommand() {}
    void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand {
 public:
    GetCurrDirCommand(const char* cmd_line);
    virtual ~GetCurrDirCommand() {}
    void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
 public:
    ShowPidCommand(const char* cmd_line);
    virtual ~ShowPidCommand() {}
    void execute() override;
};

class JobsList;

class QuitCommand : public BuiltInCommand {
// TODO: Add your data members public:
    QuitCommand(const char* cmd_line, JobsList* jobs);
    virtual ~QuitCommand() {}
    void execute() override;
};

class CommandsHistory {
 protected:
  class CommandHistoryEntry {
	  // TODO: Add your data members
  };
 // TODO: Add your data members
 public:
    CommandsHistory();
    ~CommandsHistory() {}
    void addRecord(const char* cmd_line);
    void printHistory();
};

class HistoryCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
    HistoryCommand(const char* cmd_line, CommandsHistory* history);
    virtual ~HistoryCommand() {}
    void execute() override;
};

class JobsList {
 public:
  class JobEntry {
  public:
      pid_t pid;
      bool stopped;
      int job_id;
      time_t time_added;
      explicit JobEntry(pid_t pid_in, bool stopped_in, int job_id_in, time_t time_added_in) : pid(pid_in),
                                    stopped(stopped_in), job_id(job_id_in), time_added(time_added_in) {};
      bool operator<(JobEntry& jobEntry) const {return job_id < jobEntry.job_id;}
  };
 // TODO: Add your data members
 list<JobEntry*> jobsList;
 public:
    JobsList();
    ~JobsList() {};
    void addJob(Command* cmd, bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry * getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry * getLastJob(int* lastJobId);
    JobEntry *getLastStoppedJob(int *jobId);
    // TODO: Add extra methods or modify exisitng ones as needed
};

class JobsCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
    JobsCommand(const char* cmd_line, JobsList* jobs);
    virtual ~JobsCommand() {}
    void execute() override;
};

class KillCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
    KillCommand(const char* cmd_line, JobsList* jobs);
    virtual ~KillCommand() {}
    void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
    ForegroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~ForegroundCommand() {}
    void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
    BackgroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~BackgroundCommand() {}
    void execute() override;
};


// TODO: should it really inhirit from BuiltInCommand ?
class CopyCommand : public BuiltInCommand {
 public:
    CopyCommand(const char* cmd_line);
    virtual ~CopyCommand() {}
    void execute() override;
};

class ChangePrompt : public BuiltInCommand {
public:
    ChangePrompt(const char* cmd_line);
    virtual ~ChangePrompt() {}
    void execute() override;
};

// TODO: add more classes if needed
// maybe chprompt , timeout ?

class SmallShell {

private:
    // TODO: Add your data members
    SmallShell();

    const string def_prompt = "smash";
    string prompt;
    string last_pwd;
    int curr_fd;
    JobsList jobsList;
public:
    Command *CreateCommand(const char *cmd_line);

    SmallShell(SmallShell const &) = delete; // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator
    static SmallShell &getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    ~SmallShell();

    void executeCommand(const char *cmd_line);

    void changePrompt(string new_prompt);

    void changeCurrFd(int new_fd);

    int getCurrFd() const;

    void changeLastPwd(string new_pwd);

    string getLastPwd() const;

    string getPrompt() const;
    // TODO: add extra methods as needed
};

static SmallShell& smash = SmallShell::getInstance();



#endif //SMASH_COMMAND_H_
