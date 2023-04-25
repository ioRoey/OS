#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
using namespace std;
typedef enum { FALSE , TRUE } bool_t;


class Job {
public:
	int job_id;
	char const *command;
	pid_t pid;
	time_t init_time;
	bool stopped;
public:
	Job(int job_id,const char* command, pid_t pid);
	void update_init_time();

};

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, vector<Job>& jobs);
int ExeCmd(vector<Job>& jobs, char* lineSize, char* cmdString);
int ExeExternal(char *args[MAX_ARG], char* cmdString, bool is_bg = false);
#endif

