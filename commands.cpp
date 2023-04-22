//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************


//	global variables
char* prev_dir = NULL;



Job::Job(int job_id,const char* command, pid_t pid){
	this->job_id = job_id;
	this->command = command;
	this->pid = pid;
	this->init_time = time(NULL);
}
void Job::update_init_time(){
	this->init_time = time(NULL);
}



////////////////////////

int ExeCmd(vector<Job> jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	const char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
	const char* path;


/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		if(num_arg > 1){

			cout << "smash error: cd: too many arguments" << endl;
		}
		else if (!strcmp(args[1],"-")) {
			if(!prev_dir) {
				cout << "smash error: cd: OLDPWD not set" << endl;
				return 0;
			}
			else {
				path = prev_dir;
			}
		}
		else {
			prev_dir = get_current_dir_name();
			if(!prev_dir){
				perror("smash error: get_current_dir_name failed");
			}
			path = args[1];
		}
		if(chdir(path)){
			perror("smash error: chdir failed");
		}
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		char* cwd = get_current_dir_name();
		if(!cwd){
			perror("smash error: cwd failed");
		}
		else {
			cout << get_current_dir_name() << endl;
		}

	}
	
	/*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
//		Job eden = Job("Ayelet",123);
//		eden.job_id = 5;
//		eden.stopped = true;
//		jobs.push_back(eden);
//		cout << jobs.size() << endl;
		for(const auto& job :jobs ){
//			cout << "entered eden" << endl;
			cout << "[" <<job.job_id << "] " <<
					job.command << " " << job.pid << " " << time(NULL) - job.init_time <<
					" sec";
			if(job.stopped){
				cout << " (stopped)" << endl;
			}
			else {
				cout << endl;
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		cout << "smash pid is " << getpid() << endl;

	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{

  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		int pID = ExeExternal(args, cmdString);

 		if(!strcmp(args[num_arg-1],"&")){
 			if(jobs.size()==0){
 				Job bg_job = Job(1,cmdString,pID);
 			}
 			jobs.push_back(bg_job);
 		}
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
int ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork())
	{
    		case -1:
					// Add your code here (error)
    				perror("smash error: fork failed");
        	case 0 :
                	// Child Process
               		setpgrp();
               		// Add your code here (execute an external command)
               		execv(cmdString, args);
               		perror("smash error: execv failed");

			default:
                	return pID;

	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, vector<Job> jobs)
{

	char* Command;
	const char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}

