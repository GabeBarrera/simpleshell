/** 
 * TOKENIZER.C
 * 
 * Gabriel Barrera
 * Tara Vu
 * Professor Roch
 * CS 570
 * Due Date: Thursday 02/21/2019
 */

#include "tokenizer.h"
#include <readline/readline.h>
#include <readline/history.h>

/** 
 * PUSH
 * 
 * Takes in head node pointer and string,
 * and stores in LL appropriately.
 */
void push(token_node * head, char * word) {
	// If first node is empty, store in first node
	if (head->word == NULL)
		head->word = word;
	
	// Else, create a new node and store data in new node
	else {
		// Store head to start at LL beginning
		token_node * current = head; 
		// Iterate through next pointers until the tail
		while (current->next != NULL)
			current = current->next;

		// Create new token tail node of LL 
		current->next = (token_node *)malloc(sizeof(token_node));
		// Store string in new tail token
		current->next->word = word;
		// Set tail token next pointer to NULL
		current->next->next = NULL;
	}
}

/** 
 * OUTPUT
 * 
 * Takes in head node and iterate through
 * the nodes printing their data
 */
void output(token_node * head) {
    token_node * current = head;
	// Unless head is null, iterate through LL
	if (strcmp(current->word,"\0")!=0){
		printf("["); // Start print bracket
		// Iterate through LL until tail token
		while (current != NULL) {
			// If last token, no comma after
			if (current->next==NULL)
				printf("{%s}", current->word);
			// If not last token, print comma after
			else 
				printf("{%s},", current->word);
			current = current->next; // Move up LL node
		}
		printf("]\n"); // End print bracket 
	}
}

/**
 * DELETE LIST
 * 
 * Takes in a token head and frees
 * the allocated memory of the LL.
 */ 
void delete_list(token_node * head){
	token_node * current = head;
	token_node * next;

	// Save next node, delete current
	while (current != NULL)  { 
       next = current->next; 
       free(current); 
       current = next; 
   	} 
   	head = NULL;
}

/**
 * FINDWORD 
 * 
 * Takes in head node. Reads in user input and builds
 * word tokens based on the below rule set. On word
 * completion, word is pushed to tail LL node. On
 * EOF or newline, returns a -1 to terminate program.
 */
int findword(token_node *head){
	// STRING MANIPULATION & WORD BUILDING VARIABLES
	char wordArray[MAXWORD];	 // Word Builder storage
	char *input = readline(">"); // Line Reader storage

	// COUNTERS & FLAGS
	int wordCount = 0;	// Counter for wordArray
	int inCount = 0;	// Counter for input pointer
	int quotes1 = 0;	// Flag for single quotes
	int quotes2 = 0;	// Flag for double quotes 
	int escape = 0;		// Flag for backslash 
	
	// While the current character is not EOF/Newline
	if (input != NULL){
		// Iterate through characters in *input
		for (inCount = 0; inCount< strlen(input); inCount++){

			// SPACE HANDLER
			if(isspace(input[inCount])){
				/*If quotes or backslash is active, 
				  add space to word */
				 
				if (quotes1 == 1 || quotes2 == 1 || escape == 1){
					// Append char to word
					wordArray[wordCount++] = input[inCount]; 
					// Turn off escape flag (no longer needed)
					escape = 0; 
				}
				/* Else, push word to linked list 
				   and build new word */
				else if (wordCount != 0){
					// Null terminate word
					wordArray[wordCount] = '\0';
					// Push to linked list
					push(head,strdup(wordArray)); 
					// Clear array for next word
					memset(wordArray,'\0',100);   
					// Reset wordCount for next word count
					wordCount = 0; 
				}
			}

			// SPECIAL CHARACTERS HANDLER
			else if (input[inCount]=='|' || input[inCount]==';'
			|| input[inCount]=='<' || input[inCount]=='>' 
			|| input[inCount]=='&') {
				/* If single/double quotes or escape 
				   flags are on, append special char */
				if (quotes1 == 1 || quotes2 == 1 || escape == 1){
					// Append char to word
					wordArray[wordCount++] = input[inCount]; 
					// Turn off escape flag (no longer needed)
					escape = 0; 
				}
				/* Else, push word to linked list, 
				   push special char, and start new word */
				else{
					/* If a word exists prior to this char, 
					   push word to an LL node */
					if (wordArray[0] != '\0'){
						// Null terminate word
						wordArray[wordCount] = '\0';
						// Push to linked list  
						push(head,strdup(wordArray)); 
						// Clear array for next word
						memset(wordArray,'\0',100); 
						// Reset wordCount for next word count  
						wordCount = 0; 
					}

					// Push special char to its own LL node
					// Append char to word
					wordArray[wordCount] = input[inCount]; 
					// Push to linked list
					push(head,strdup(wordArray)); 
					// Clear array for next word
					memset(wordArray,'\0',100); 
					// Reset wordCount for next word count  
					wordCount = 0; 
				}
			}

			// BACKSLASH HANDLER
			else if (input[inCount]=='\\'){
				/* If single quotes/backslash is active, 
				   add backslash to word */ 
				if (quotes1 == 1 || escape == 1){
					// Append char to word
					wordArray[wordCount++] = input[inCount]; 
					// Turn off escape flag (no longer needed)
					escape = 0; 
				}

				/* Else, turn on escape flag. 
				   Next char will be appended to word */ 
				else escape = 1;
			}

			// SINGLE QUOTES HANDLER
			else if (input[inCount]=='\''){
				/* If double quotes flag or escape 
				   flag is on, add quote to word */ 
				if (quotes2 == 1 || escape == 1){
					// Append char to word
					wordArray[wordCount++] = input[inCount]; 
					// Turn off escape flag (no longer needed)
					escape = 0; 
				}

				/* If single quotes is off, turn on.
				   Ignore special rules, append to word */ 
				else if (quotes1 == 0)
					quotes1 = 1;

				/* If single quotes is on, turn off.
				   Proceeding chars will be appended 
				   to word normally */
				else if (quotes1 == 1)	
					quotes1 = 0;
			}

			// DOUBLE QUOTES HANDLER
			else if (input[inCount]=='\"'){
				/* If double quotes flag/escape flag is on, 
				   add quote to word */
				 
				if (quotes1 == 1 || escape == 1){
					// Append char to word
					wordArray[wordCount++] = input[inCount]; 
					// Turn off escape flag (no longer needed)
					escape = 0; 
				}
 
				/* If double quotes is off, turn on.
				   Proceeding chars will be appended 
				   to word ignoring special rules */
				else if (quotes2 == 0)
					quotes2 = 1;

				/* If single quotes is on, turn off.
				   Proceeding chars will be appended 
				   to word normally */
				else if (quotes2 == 1)	
					quotes2 = 0;
			}

			// APPEND CHARACTER
			else 
				/* If char doesn't apply to rule 
				   set, append to word */
				wordArray[wordCount++] = input[inCount];
		}

		// If last word exists, push it to LL node
		if (wordArray[0]!='\0'){
			// Null terminate word
			wordArray[wordCount] = '\0';
			// Push to linked list 
			push(head,strdup(wordArray)); 
		}
		// Clear word array
		memset(wordArray,'\0',100); 
		// Reset word counter
		wordCount = 0; 
		// Reset input for next readline
		input = NULL; 
		// Default Return Value --> Continue Program
		return 0; 
	}
	// EOF Program Value --> Terminate Program
	return -1; 
}

// Explicit instantiation of execute_commands()
void execute_commands(token_node * head);

/**
 * MAIN
 * 
 * Initialize linked list, run MUSH functions
 */ 
int main(){
	// Create new Linked List, initialize, mem allocation
	token_node *head = NULL; // Initialize node head
	head = (token_node *) malloc(sizeof(token_node)); 
	head->word=NULL; // Initialize word value
	head->next=NULL; // Initialize next pointer

	/* The findword() returns an integer value. The
	   return value is stored here in 'terminate'.
	   If the returned value is -1, the for loop breaks
	   and the program terminates. */
	int terminate = 0;
	
	// Repeat prompt and terminate on EOF
	for(;;){
		// Find words to store in Linked List
		terminate = findword(head);
		
		// If head word is not null, print linked list
		if (head->word != NULL){
			execute_commands(head); // Execute commands
			output(head); // Prints linked list
			delete_list(head->next);// Delete all but first node	
		}
		// Reset Linked List for next input line
		head->word = NULL;
		head->next = NULL;
		// Exit Loop: terminates program on -1 return
		if (terminate == -1)
			break;
	}
	// Deallocate all memory
	delete_list(head);
}