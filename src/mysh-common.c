#include "mysh.h"

int main(void) {
	printf("\nmyshell> ");
	int x = readCommand();
	while(x == 1){
		printf("\nmyshell> ");
		x = readCommand();
	}
		return 0;
}

// Reads the command
int readCommand(){
	char command[258];
	int input = (int)fgets(command, sizeof(command), stdin); // fgets reads the input from the user.
	int x = strlen(command) -1;	
	
	/* Case NULL means that the user typed Ctrl + D
	 * Default means that the user typed something.
	 */
	switch(input){ 
		case (int) NULL: {
			return 0;
			break;
		}
		default: {  /* Checks if the input surpassed 255 characters or not and ends the program if
			     * it does else it tries to execute the command.
			     */			
			if (x == 256) {
				printf("%s","Max characters 255! Please try again.\n");
				return 0;
			}else{
				execute(command);
				return 1;
			}
			break;
		}
	}		
}	


/* If the command is valid it tries to execute it.
 * n is the command and k is the number of the shell.
 */
void execute(char *n){
	
	// We create a child process.
	pid_t pid, waitPid;
	int  status;
	pid = fork();
	
	if (pid < 0) {
		perror("ERROR: Fork failed.\n");		
	}

	// Child.
	if (pid == 0) {	
		
		int i = 0;
		int z = 0;
		int k = 0;
		int ind1 = 0;
		int ind2 = 0;
		char *fnames[20] = {0};
		char *check;
		char *toks[20] = {0}; // Tokens with "|" as delimiter.
		char *cmd[20] = {0}; // Token with " " as delimiter.
		n[strlen(n)-1] = '\0';
		
		
		check = strtok(n, "|");				
		
		// Stores everything but "|" in toks[].
		while(check != NULL){
			
			toks[i] = check;
			i++;
			check = strtok(NULL, "|");
		}

		int fd[2]; // fd[0] : pipe input , fd[1] : pipe outpout.
		int fd_in = 0;
		pid_t pid1;
		int  status1;
		
		// While there is next command.
		while(toks[k] != NULL){
			int j = 0; // For cmd[].
			int flag1 = 0; // If we find "<" we change flag1 to 1.

			int flag2 = 0; // If we find ">" we change flag2 to 1.
					   
			check = strtok(toks[k], " ");
		
			// Seperates commands from file names and "<", ">".
			while(check != NULL){

				if(strcmp(check,"<") != 0 && strcmp(check,">") != 0){
					
					cmd[j] = check;
					j++;
				}	
			
				
				if(strcmp(check,"<") == 0){

					flag1 = 1;
					ind1 = z; // Position of the file name in fnames[].
					fnames[z] = strtok(NULL, " "); // Name of the file.
					z++;	
				}

				
				if(strcmp(check,">") == 0){

					flag2 = 1;
					ind2 = z; // Position of the file name in fnames[].
					fnames[z] = strtok(NULL, " "); // Name of the file.
					z++;
				}
			
				check = strtok(NULL, " "); // Next token.
			}
		
			pipe(fd);
			pid1 = fork();

			if(pid1 == -1){ // If fork fails.
				exit(EXIT_FAILURE);
			}
			if(pid1 == 0){ // Child.
				
				// Opens the file to read from.
				if(flag1 == 1){

					openFile(fnames[ind1], 0);
				}

				// Opens the file to write to.
				if(flag2 == 1){

					openFile(fnames[ind2], 1); 
				}
				
				/* If there is not an input from a file then
				 * changes the input to the output of the last command.
				 */
				if(flag1 == 0){
					dup2(fd_in, 0);
				}

				// If not last command.
				if(toks[k +1] != NULL){
					/* If there is ">" in the left part of the pipe 
					 * then we dont duplicate the fd[1]
					 */
					if(flag2 == 0){
						dup2(fd[1],1);
					}
				}
				close(fd[0]);
				execvp(cmd[0], cmd);
				perror("Command not found ");
				exit(EXIT_FAILURE);
		
			} //End child.

			else{ // Father.

				wait(&status1);
				close(fd[1]);
				fd_in = fd[0]; //Saves the input for the last command.
				k++;
				for(i = 0; i < j; i++){ // Reinitializes cmd.
					cmd[i] = NULL;
				}

			} // End father.				
		} // End while.
		exit(EXIT_SUCCESS);
	} // End child.

	// Father.
	else{	
		// Father waits.
		waitPid = wait(&status);
		if (waitPid == -1) {
			perror("ERROR: Waitpid failed.\n");
		}
	}
} // End readCommand().

// Opens a file to read from or write to, x is 1 or 0.
void openFile(char *filename, int x){
	
	// x means we open a file to write to. If the file doesnt exist we create it.
	if(x == 1){

		int file = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);	

		if(dup2(file,1) < 0){
			perror("ERROR dup2 failed ");
		}
	}

	// x means we open a file to read from. If the file doesnt exist we throw error.
	else{
		int file = open(filename, O_RDONLY);

		if(file < 0){
			
			perror("ERROR ");
		}

		if(dup2(file,0) < 0){
			
			exit(EXIT_FAILURE);
		}
	}
}


