// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"


void catch_int (int sig_num) {
	std::cout << "\nsmash: caught ctrl-C" << std::endl;
//	pid_t pid = get_id();
	std::cout << "smash: process " << getpid() <<" was killed"  <<std::endl;
	kill(getpid(), SIGKILL);
}

void catch_stop (int sig_num) {
	kill(getpid(), SIGSTOP);
}