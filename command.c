/**
 * COMMAND.C
 *  
 * Gabriel Barrera
 * Tara Vu
 * Professor Roch
 * CS 570
 * Due Date: Thursday 02/21/2019
 */

#include "tokenizer.h"

// Initialize variables
size_t size = 1000;
pid_t kid = 0; 
char * currentDir;

// Error Strings
char * cdfail = "Directory does not exist or is not accessible";
char * cderr = "Accepts exactly one argument";
char * ioerr = "IO redirection and background not implemented";
char * piperr = "Pipe not implemented";
char * argerr = "ERROR: Too many arguments";

/**
 * DELETE
 * 
 * Takes in head node pointer, and 
 * deletes a node from the LL
 */
void delete(token_node * token){
	/* Unless linked list is null, delete a
	 * node, set token to deleted node's next
	 */
	if (token->next != NULL){
		token_node * tmp = token->next;
		token->word = token->next->word;
		token->next = tmp->next;
	}		
}

/**
 * COMMANDS HELPER
 * 
 * Takes in an argv[]. Looks for a CD or PWD command
 * and executes the command appropriately. 
 */
void commands_helper(char * argv[]){
	// CD COMMAND - change directory
	if (strcmp(argv[0],"cd")==0){
		/**
		 * If cd has a next node and its 
		 * next node does not exist,
		 * change to directory of the 
		 * second node's word data
		 */ 
		if (argv[1] != NULL && argv[2] == NULL){
			 // Check if directory exists
			DIR * dir = opendir(argv[1]);
			if (dir)
				// Change directory
				chdir(argv[1]); 
			else 
				printf("%s\n",cdfail);
		}

		/**
		 * If cd has a next node, and its 
		 * next node has a next node, 
		 * there are an incorrect amount 
		 * of arguments.
		 */
		else 
			printf("%s\n",cderr);
	}

	// PWD COMMAND - print current working directory
	else if (strcmp(argv[0],"pwd")==0 && argv[1] == NULL)
		printf("%s\n",getcwd(currentDir,size));

	// EXECVP - execute any other commands
	else{
		fflush(stdout); // Clear stdout
		fflush(stderr); // Clear stderr
		int status; // Wait status variable
		kid = fork(); // Fork process
		// If an error occurred, print out error message
		if (kid < 0){
			printf("Unable to execute %s\n",*argv);
			exit(1);
		}
		// If child process, execute command
		if (kid == 0) {
			if (execvp(*argv, argv) < 0) {
               printf("*** ERROR: exec failed\n");
               exit(1);
          	}
		// If parent process, wait for child
		} else {
			while (wait(&status) != kid);
		}
	}
}

/**
 * EXECUTE COMMANDS
 * 
 * Takes in token head and iterates
 * through found words. Builds argv
 * array with a command and its
 * arguments. Upon delimiter or end 
 * of line, calls commands_helper() 
 * to execute the argv array
 * 
 * commands_helper function 
 */
void execute_commands(token_node *head){
	token_node * current = head; // Current Head
	token_node * lhead = head; // Local Head
	int argCount = 0; // Args Array counter
	char * argv[MAXARGS]; // Args Array
	
	// Iterate through all commands
	while (current != NULL){
		/* Keep appending commands arguments 
		   to the argv array until MAXARGS */
		if (argCount < MAXARGS){
			
			// If ; or |
			if (strcmp(current->word,";")==0 || 
				strcmp(current->word,"|")==0){

				if (strcmp(current->word,"|")==0)
					printf("%s\n",piperr);
				argv[argCount] = NULL;
				/* Send argv array to command 
				   helper to execute commands */
				if (argCount > 0)
					commands_helper(argv);
				// Reset Word Count
				argCount = 0;
			}

			// If <, >, or &
			else if (strcmp(current->word,"<")==0 || 
				strcmp(current->word,">")==0 ||
				strcmp(current->word,"&")==0){

				if (strcmp(current->word,"&")!=0){
					/* If redirect arg exists 
					 and another token exists 
					 after it */
					if (current->next!=NULL && 
						current->next->next != NULL){

						// Delete redirect					
						delete(current);
						// Delete arg
						delete(current);
						// Move to next token
						if (current->next != NULL)
							current = current->next;
					}
					/* If redirect arg exists 
					   and no new tokens 
					   exists after it */
					else if (current->next!=NULL && 
						current->next->next == NULL){

						// End LL at this node
						current = lhead;
						lhead->next = NULL;
					}
					// < or > is the only token
					else 
						current->word = "\0";
				}
				// If &
				else if 
				(strcmp(current->word,"&")==0){
					/* If another token exists 
					   after it */
					if (current->next!=NULL){

						// Delete &
						delete(current);
						// Move to next token
						if (current->next != NULL)
							current = current->next;
					}
					/* If no new tokens exists 
					   after it */
					else if 
					(current->next==NULL){
						/* Set previous 
						   node as tail */
						current->word = "\0";
						if (current != head)
							lhead->next=NULL;					
					}
				}
				printf("%s\n", ioerr);

				argv[argCount] = NULL;
				/* Send argv array to command 
				   helper to execute commands */
				if (argCount > 0)
					commands_helper(argv);
				// Reset word count
				argCount = 0;	
			}

			// Append to args array
			else
				argv[argCount++] = strdup(current->word);
		}

		else{
			printf("%s\n",argerr);
			argCount = 0;
			/* If command exceeds length 
			   skip to next delimiter */
			while (strcmp(current->word,";")!=0 && 
				strcmp(current->word,"|")!=0 &&
				strcmp(current->word,"<")!=0 && 
				strcmp(current->word,">")!=0)
				current = current->next;
		}

		// Set Local Head
		if (strcmp(current->word,"<")==0 || 
			strcmp(current->word,">")==0 ||
			strcmp(current->word,"&")==0){
			lhead = current;
		}

		// Point to next token
		current = current->next;
	}

	// If end of readline, execute last 
	if (argv[0] != NULL){
		argv[argCount] = NULL;
		/* Send argv array to command 
		   helper to execute commands */
		if (argCount > 0)
			commands_helper(argv);
		// Reset word count
		argCount = 0;	
	}
}