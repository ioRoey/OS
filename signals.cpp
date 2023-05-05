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
		invalid_fg_pid();
		if(kill(pid, SIGKILL) == -1){
			perror("smash : kill failed");
		}

	}
}

void catch_stop (int sig_num) {
    std::cout << "smash: caught ctrl-Z" << std::endl;
	pid_t pid = get_fg_pid();
	if (pid != -1) {
		cout << "smash: process " << pid << " was stopped" << endl;
		insert_fg_to_jobs(); // finish after convert smash to class
	    invalid_fg_pid();
		if(kill(pid, SIGSTOP) == -1 ){
			perror("smash: kill failed");
		}
		return;

	}
}
