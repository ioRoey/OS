//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************





Job::Job (int job_id,const char* command, pid_t pid) {
	this->job_id = job_id;
	strcpy(this->command,command);
	this->command[strlen(command)-1] = '\0';
	this->pid = pid;
	this->init_time = time(NULL);
	this->stopped = false;
}

Job::Job (const Job &other) {
	this->job_id = other.job_id;
	strcpy(this->command ,other.command);
	this->pid = other.pid;
	this->init_time = other.init_time;
	this->stopped = other.stopped;
}

void Job::update_init_time(){
	this->init_time = time(NULL);
}


//	global variables
char* prev_dir = NULL;
Job fg_job = Job(-1,"",-1);
vector<Job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)


// helper functions

int find_next_job_id(){
	if(jobs.empty()){
		return 1;
	}
	int ret = jobs.back().job_id;
	return ++ret;

}

void insert_fg_to_jobs() {
	fg_job.stopped =  true;
	fg_job.update_init_time();
	if (fg_job.job_id == -1) {
		fg_job.job_id = find_next_job_id();

		jobs.push_back(fg_job);
		return;
	}
	jobs.insert(jobs.begin() + (fg_job.job_id-1), fg_job);
}

void remove_zombie_jobs(vector<Job>& jobs){
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

////////////////////////

int ExeCmd(char* lineSize, char* cmdString) // vector<Job>& jobs - FIX
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
 		if(num_arg != 2){
 			perror("smash error: diff: invalid arguments");
 			return 0;
 		}
 		FILE* f1 = fopen(args[1],"r");
 		FILE* f2 = fopen(args[2],"r");
 		char ch1,ch2;

		do {
			ch1 = fgetc(f1);
			ch2 = fgetc(f2);
			if(ch1 != ch2){
				cout << "1" << endl;
				fclose(f1);
				fclose(f2);
				return 0;
			}

		} while (ch1 != EOF && ch2 != EOF);
		if((ch1 == EOF && ch2 != EOF) || (ch2 == EOF && ch1 != EOF)){
			cout << "1" << endl;
		}
		cout << "0" << endl;
		fclose(f1);
		fclose(f2);
	}

	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		remove_zombie_jobs(jobs);
		for(const auto& job :jobs ){
			cout << "[" <<job.job_id << "] " <<	job.command << " : " << job.pid << " " << difftime(time(NULL),job.init_time) <<
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
	else if (!strcmp(cmd, "fg")) {
		vector<Job>::iterator it;
		if (num_arg == 0)
		{
			if(jobs.empty()){
				perror("smash error: fg: jobs list is empty");
				return 0;
			}

			fg_job = Job(jobs.back());
			// print the full command
			cout << fg_job.command << " : "<< fg_job.pid << endl;


			if (kill(fg_job.pid,SIGCONT) == -1) {
				perror("smash error: kill failed");
			}

			it = jobs.begin() + fg_job.job_id-1;
			jobs.erase(it);
			pid_t pid = waitpid(fg_job.pid,NULL,WUNTRACED);
			if(pid == -1){
				perror("smash error: waitpid failed");
			}
			invalid_fg_pid();

			return 0;
		}
		// copy the fields of the job to fg_job
		string job_id = args[1];
		string::const_iterator it1 = job_id.begin();
		while (it1 != job_id.end() && std::isdigit(*it1)) ++it1;

		if ((!job_id.empty() && it1 != job_id.end()) || num_arg > 1){
			perror("smash error: fg: invalid arguments");
			return 0;
		}

		for (const auto& job : jobs) {
			if (job.job_id == stoi(job_id)){
				fg_job = Job(job);

				// print the full command
				cout << fg_job.command << " : " << fg_job.pid << endl;

				if (kill(fg_job.pid,SIGCONT) == -1) {
					perror("smash error: kill failed");
				}
				it = jobs.begin() + fg_job.job_id-1;
				jobs.erase(it);

				pid_t pid = waitpid(fg_job.pid,NULL,WUNTRACED);
				if (pid == -1){
					perror("smash error: waitpid failed");
				}
				invalid_fg_pid();
				return 0;
			}
		}
		string err = "smash error: fg: job-id " + job_id + " does not exist";
		perror(err.c_str());
	} 

	/*************************************************/
	else if (!strcmp(cmd, "bg")) {


		if (num_arg == 0) {

			// find the "highest" stopped job using a reversed loop
			auto it = jobs.end();
			while (1) {
				if (it->stopped == true) {
				   if (kill(it->pid,SIGCONT) == -1) {
					   perror("smash error: kill failed");
				   }

					// print the full command
					cout << it->command << " : " << it->pid << endl;

					// activate the job in the bg
					if (kill(it->pid,SIGCONT) == -1) {
						perror("smash error: kill failed");
					}

					it->stopped = false;
					return 0;
				}

				if (it == jobs.begin()) break;
				it--;
			}

			string err = "smash error: there are no stopped jobs to resume";
			perror(err.c_str());
			return 0;
		}
		string job_id = args[1];
		string::const_iterator it1 = job_id.begin();
		while (it1 != job_id.end() && std::isdigit(*it1)) ++it1;

		if ((!job_id.empty() && it1 != job_id.end()) || num_arg > 1){
			perror("smash error: bg: invalid arguments");
			return 0;
		}

		for (auto& job : jobs) {
			if (job.job_id == stoi(job_id)){
				if (job.stopped == false) {
					string err = "smash error: bg: job-id " + job_id + " is already running in the background";
					perror(err.c_str());
				}
				if (kill(job.pid,SIGCONT) == -1) {
					perror("smash error: kill failed");
				}

				// print the full command
				cout << job.command << " : " << job.pid << endl;

				// activate the job in the bg
				if (kill(job.pid,SIGCONT) == -1) {
					perror("smash error: kill failed");
				}

				job.stopped = false;
				return 0;
			}
		}
		string err = "smash error: bg: job-id " + job_id + " does not exist";
		perror(err.c_str());
	}

	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if (num_arg != 0) {
			if (!strcmp(args[1],"kill")){
				for (const auto& job : jobs) {
					if( kill(job.pid,SIGTERM) == -1) {
						perror("smash error : kill failed");
					}
					cout << "[" <<job.job_id << "] " <<	job.command << " - Sending SIGTERM...";
					sleep(5);
					pid_t pid = waitpid(job.pid,NULL,WNOHANG);
					if(pid == -1){
						perror("smash error: waitpid failed");
					}
					else if(pid == 0) {
						if(kill(job.pid,SIGKILL) == -1){
							perror("smash error : kill failed");
						}
						cout << " (5 sec passed) Sending SIGKILL... DONE" << endl;
					}
					else {
						cout << " DONE" <<endl;
					}

				}
			}
		}

   		if (kill(getpid(),SIGKILL) == -1){
			perror("smash error: kill failed");
		}
		return 0;

	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill")) {
		if (num_arg != 2) {
			perror("smash error: kill: invalid arguments");
			return 0;
		}

		string sig_num = args[1];
		string job_id = args[2];

		string::const_iterator it1 = sig_num.begin();
		it1++; // skip the '-' char
		string::const_iterator it2 = job_id.begin();

		while (it1 != sig_num.end() && std::isdigit(*it1)) ++it1;
		while (it2 != job_id.end() && std::isdigit(*it2)) ++it2;

		if ( (*(args[1]) != '-') ||
				(!sig_num.empty() && it1 != sig_num.end()) ||
				(!job_id.empty() && it2 != job_id.end()) ){
			perror("smash error: kill: invalid arguments");
			return 0;
		}

		for (const auto& job : jobs) {
			if (job.job_id == stoi(job_id)){
				if (kill(job.pid,stoi(sig_num)*(-1)) == -1) {
					perror("smash error: kill failed");
				}
				cout << "signal number " << stoi(sig_num)*(-1) << " was sent to pid " << job.pid << endl;
				return 0;
			}
		}

		string err = "smash error: kill: job-id " + job_id + " does not exist";
		perror(err.c_str());
	}


	/*************************************************/
	else // external command
	{
 		int pID = ExeExternal(cmdString,args, cmd);


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
int ExeExternal(char* full_command, char *args[MAX_ARG], char* cmdString,bool is_bg)
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
               		return 0;

			default:
					if(!is_bg){
						fg_job.pid = pID;
						fg_job.job_id = -1;
						strcpy(fg_job.command,full_command);
						fg_job.update_init_time();
						pid_t pid = waitpid(pID,NULL,WUNTRACED);
						if (pid == -1){
							perror("smash error: waitpid failed");
						}
						invalid_fg_pid();
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
int BgCmd(char* lineSize) //vector<Job>& jobs - FIX
{

	char* Command;
	const char* delimiters = " \t\n";
	char *args[MAX_ARG];
	int max_job_id, num_arg = 0;

    char full_command[MAX_LINE_SIZE];

	if (lineSize[strlen(lineSize)-2] == '&')
	{
		strcpy(full_command, lineSize);
		// Add your code here (execute a in the background)
		lineSize[strlen(lineSize)-2] = '\0';
		Command = strtok(lineSize, delimiters);
		if (Command == NULL)
				return 0;
		args[0] = Command;
		for (int i=1; i<MAX_ARG; i++) {
			args[i] = strtok(NULL, delimiters);
			if (args[i] != NULL)
				num_arg++;
		}
		int pID = ExeExternal(full_command,args,Command,true);
		remove_zombie_jobs(jobs);
		max_job_id = find_next_job_id();
		Job bg_job = Job(max_job_id,full_command,pID);
		jobs.push_back(bg_job);
		return 0;
	}
	return -1;
}

pid_t get_fg_pid(){
	return fg_job.pid;
}

void invalid_fg_pid(){
	fg_job.pid = -1;
	fg_job.job_id = -1;
	fg_job.stopped =  false;
}


