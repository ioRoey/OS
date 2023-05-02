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
	char command[MAX_LINE_SIZE];
	pid_t pid;
	time_t init_time;
	bool stopped;
public:
	Job(int job_id,const char* command, pid_t pid);
	Job(const Job &other);
	void update_init_time();

};


int ExeComp(char* lineSize);
int BgCmd(char* lineSize);
int ExeCmd(char* lineSize, char* cmdString);
int ExeExternal(char* full_command,char *args[MAX_ARG], char* cmdString, bool is_bg = false);
pid_t get_fg_pid();
void invalid_fg_pid();
void insert_fg_to_jobs();
#endif

