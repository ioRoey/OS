// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */

#include "signals.h"


void catch_int (int sig_num) {
	std::cout << "smash: caught ctrl-C" << std::endl;
	pid_t pid = get_fg_pid();
	if (pid != -1) {
		cout << "smash: process " << pid << " was killed" << endl;
		kill(pid, SIGKILL);
		invalid_fg_pid();
	}
}

void catch_stop (int sig_num) {
    std::cout << "smash: caught ctrl-Z" << std::endl;
	pid_t pid = get_fg_pid();
	if (pid != -1) {
		cout << "smash: process " << pid << " was stopped" << endl;
		kill(pid, SIGSTOP);
		insert_fg_to_jobs(vector<Job>& jobs) // finish after convert smash to class
		invalid_fg_pid();
	}
}
