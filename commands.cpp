//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************





Job::Job(int job_id,const char* command, pid_t pid){
	this->job_id = job_id;
	this->command = command;
	this->pid = pid;
	this->init_time = time(NULL);
	this->stopped = false;
}
void Job::update_init_time(){
	this->init_time = time(NULL);
}

int find_next_job_id(vector<Job> jobs){
	if(jobs.empty()){
		return 1;
	}
	return ++jobs.back().job_id;

}

void remove_finished_jobs(vector<Job>& jobs){
	vector<Job>::iterator it;
	for(const auto& job : jobs){
		pid_t pid = waitpid(job.pid,NULL,WNOHANG);
		if(pid == -1){
			perror("smash error: waitpid failed");
		}
		else if(pid == 0) {
			continue;
		}
		else {
		    it = jobs.begin() + job.job_id-1;
		    jobs.erase(it);
		}

	}

}
//	global variables
char* prev_dir = NULL;
Job fg_job = Job(-1,"",-1);
////////////////////////

int ExeCmd(vector<Job>& jobs, char* lineSize, char* cmdString)
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

			perror("smash error: cd: too many arguments");
			return 1;
		}
		else if (!strcmp(args[1],"-")) {
			if(!prev_dir) {
				perror("smash error: cd: OLDPWD not set");
				return 1;
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
		remove_finished_jobs(jobs);
		for(const auto& job :jobs ){
			cout << "[" <<job.job_id << "] " <<
					job.command << " " << job.pid << " " << difftime(time(NULL),job.init_time) <<
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
 		int pID = ExeExternal(args, cmd);


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
int ExeExternal(char *args[MAX_ARG], char* cmdString,bool is_bg)
{
	int pID;
	cout << cmdString << endl;
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
					if(!is_bg){
						waitpid(pID,NULL,NULL);
					}
                	return pID;

	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
//int ExeComp(char* lineSize)
//{
//	char ExtCmd[MAX_LINE_SIZE+2];
//	char *args[MAX_ARG];
//    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
//    {
//		// Add your code here (execute a complicated command)
//
//		/*
//		your code
//		*/
//	}
//	return -1;
//}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, vector<Job>& jobs)
{

	char* Command;
	const char* delimiters = " \t\n";
	char *args[MAX_ARG];
	int max_job_id = 1, num_arg = 0;


	if (lineSize[strlen(lineSize)-2] == '&')
	{
		// Add your code here (execute a in the background)
		lineSize[strlen(lineSize)-2] = '\0';
		Command = strtok(lineSize, delimiters);
		if (Command == NULL)
				return 0;
		args[0] = Command;
		for (int i=1; i<MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters);
			if (args[i] != NULL)
				num_arg++;
		}
		int pID = ExeExternal(args,Command,true);
		remove_finished_jobs(jobs);
		max_job_id = find_next_job_id(jobs);
		Job bg_job = Job(max_job_id,lineSize,pID);
		jobs.push_back(bg_job);
		return 0;
	}
	return -1;
}

