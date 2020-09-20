#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


int readCommand();

void execute(char *p, int k);

void openFile(char *filename, int x);
