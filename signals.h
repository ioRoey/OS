#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "commands.h"

void catch_int (int sig_num);
void catch_stop(int sig_num);
#endif

