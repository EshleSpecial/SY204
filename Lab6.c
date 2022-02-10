/*
 * Name: MIDN Eshleman
 * Alpha: 221938
 * Course: SY204 - Sys Prgm & OS Fund
 * Assignment: Lab: myShell
 */

// Required Includes
// Previous Uses: #include <libname.h>  // <Library Name>: <Uses: constants, variables, system calls, library routines>...
// #include <unistd.h>  // POSIX Standard Library: close(2), read(2), write(2)
#include <stdlib.h>  // Standard Library: EXIT_*
// New Uses
#include <unistd.h>  // POSIX Standard Library: ???
#include <stdio.h>   // Standard I/O Library: fgets(3)
#include <string.h>  // String Library: strtok_r(3)

// Global Constants
// Student: You may add constants, but shall not delete constants
// Error Constants
#define STD_ERR_RETURN     -1
#define NULL_ERR_RETURN    NULL
#define STUDENT_UPDATE_ME  1
// Length Constants
#define MAX_STR_LEN     1024
#define MAX_CMD_LN_LEN  1024
#define MAX_NUM_ARGS     128
// Read command line
#define READ_CMD_OKAY       00
#define READ_CMD_GEN_ERROR  01
#define READ_CMD_BLANK      02
// Parse flags
#define PARSE_OKAY             00
#define PARSE_GEN_ERROR        01
#define PARSE_PREV_HAND_ERROR  02

int getFullCmdLn(char *strFullCmdLn);
int tokParseFullCmdLn(char *strFullCmdLn);
int tokParseSingleCmd(char *strSingleCmd);
// Array and string utilities
int isOnlyWhiteSpace(char *strCheckForWhiteSpace);

int main( int argc, char *argv[]) {
	char *strFullCmdLn;  // String for full command line
	int callReturned = 0;  // Error checking
	int x;
    mypid = getPID();
    setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 0);
	strFullCmdLn = calloc( MAX_STR_LEN, sizeof (char) );
    int child_status;
    if(strFullCmdLn == NULL){
        perror("could not intitalize the request memory for strFullCmdLn");
    }
	while ( 1 ) {  // Command prompt Loop
		callReturned = getFullCmdLn(strFullCmdLn);
		switch (callReturned) {
			case READ_CMD_OKAY:  // Tokenize and parse command line
				callReturned = tokParseFullCmdLn(strFullCmdLn);
				switch (callReturned) {
					case PARSE_OKAY:
                        x = fork();
                        if ( x == 0){
                            printf( "path: %s\n", getenv("PATH"));
                            execlp( strFullCmdLn, strFullCmdLn, NULL);
                            printf( "strFullCmdLn [%s] \n", strFullCmdLn);
                        }
                        else {
                        wait( &child_status);
                        printf("GO");
                        }
                        break;
					default:
						fprintf(stderr, "Error detected by _h0ff\n");
						break;
				}
				break;
			default:  // Default to not executing command line
				fprintf(stderr, "Error detected by _h0ff\n");
				break;
		}
	}  // Command prompt Loop


	// Memory management
	free(strFullCmdLn);
	// Return to Caller
	return EXIT_SUCCESS;
}


int getFullCmdLn(char *strFullCmdLn) {
	/*
	 * Description: Reads in a command line from standard input
	 * Arguments:
	 *   strFullCmdLn : String representing command line read from standard input
	 * Return Value:
	 *   0 : Normal exit
	 *   # : Unable to read command line, errno set accordingly
	 * Notes:
	 *   Student: Only need to setup prompt, and update error reporting.
	 */
	// Declare Required Variables
	char *strPrompt = "";  // It's your shell
	// Perform Function


	if ( fgets(strFullCmdLn, MAX_CMD_LN_LEN, stdin) == NULL_ERR_RETURN ) {  // Read command line
		fprintf(stdout, "\n");
		fprintf(stderr, "Error detected by _h0ff\n");
		return READ_CMD_GEN_ERROR;
	}
	if (strlen(strFullCmdLn) > 1) {
		if (strFullCmdLn[strlen(strFullCmdLn) - 1] == '\n') {  // Research 2.a.
			strFullCmdLn[strlen(strFullCmdLn) - 1] = '\0';
		} else {
			fprintf(stdout, "\n");
		}
	} else {
		strFullCmdLn[0] = '\0';
		return STUDENT_UPDATE_ME;  // Change to other Defined Constant based on Research 2.a.
	}


	// Return to Caller
	return READ_CMD_OKAY;
}


int tokParseFullCmdLn(char *strFullCmdLn) {
	/*
	 * Description: Tokenizes (';' separator) and parses a full command line in individual commands
	 * Arguments:
	 *   strFullCmdLn : String representing full command line
	 * Return:
	 *   0 : Normal exit
	 *   # : ERROR: Error detected
	 * Notes:
	 *   Student: Add and modify as needed to complete Part 3 - It's a Party Line, and for error reporting
	 */
	// Declare Required Variables
	int callReturned;      // Value returned
	char *strSingleCmd;  // String representing single command
	// Perform Function


	strSingleCmd = strFullCmdLn;


	callReturned = tokParseSingleCmd(strSingleCmd);
	switch ( callReturned ) {  // Tokenizing or parsing error
		case PARSE_OKAY:   // No Error
			break;
		// Student: Add error cases as needed
		default:
			return PARSE_GEN_ERROR;
	}


	// Return to Caller
	return PARSE_OKAY;
}


int tokParseSingleCmd(char *strSingleCmd) {
	/*
	 * Description: Tokenizes (' ', '\t', '\n', '\v' separator) and parses a single command
	 * Args:
	 *   strSingleCmd : String representing single command to execute
	 * Return:
	 *   0 : Normal exit
	 *   # : ERROR: Error detected
	 * Notes:
	 *   Student: Most of your code should be in here
	 */
	// Declare Required Variables
	// Single command line
	char *strSingleCmdLnArgs[MAX_NUM_ARGS];  // Array of strings representing a single command with arguments and with redirects
	char *strSingle;         // Copy of single command to pass to strtok_r [trustno1]
	char *strToken;          // Single token
	char *bufSingleTokSave;  // Buffer for strtok_r bookkeeping
	int intNumTokens = 0;    // Number of tokens
	// Variable(s) for Process Management


	// Perform Function


	/* Begin: DO NOT MODIFY - Single Command Tokenizing */
	// If you are writing code in here, you are wrong
	// Setup strSingle; before returning need to free(strSingle)
	strSingle = calloc( MAX_STR_LEN, sizeof( char ) );  // Assume nothing
	strncpy(strSingle, strSingleCmd, MAX_CMD_LN_LEN);
	// Build strSingleCmdLnArgs, tokenize command line based on ' ', '\t', '\n', '\v'
	strToken = strtok_r(strSingle, " \t\n\v", &bufSingleTokSave);
	do {
		strSingleCmdLnArgs[intNumTokens] = strToken;
		intNumTokens++;
	} while ( (strToken = strtok_r(NULL, " \t\n\v", &bufSingleTokSave)) != NULL );
	strSingleCmdLnArgs[intNumTokens] = NULL;  // Research 2.b.
	/* End:   DO NOT MODIFY - Single Command Tokenizing */

    execv(strSingleCmdLnArgs [0], strSingleCmdLnArgs);
    
	// Memory management
	free(strSingle);
	// Return to Caller
	return PARSE_OKAY;
}

int isOnlyWhiteSpace(char *strCheckForWhiteSpace) {
	/*
	 * Description: Tests whether strCheckForWhiteSpace is only white space (' ', '\t', '\n', '\v')
	 * Arguments:
	 *   strCheckForWhiteSpace : String of up to MAX_STR_LEN that is to be checked for only having white space characters
	 * Return:
	 *   0 : strCheckForWhiteSpace IS NOT only comprised of white space characters
	 *   1 : strCheckForWhiteSpace IS     only comprised of white space characters
	 */
	// Declare Required Variables
	// White space checking
	char strWhiteSpaceCheck[MAX_STR_LEN];  // Copy of command to check for "only white space"
	char *strSansWhiteSpace;               // Command without white space
	char *bufWhiteSpaceCheck;              // Buffer for strtok_r bookkeeping
	// Perform Function
	strncpy(strWhiteSpaceCheck, strCheckForWhiteSpace, MAX_STR_LEN);  // Password: trustno1
	strSansWhiteSpace = strtok_r(strWhiteSpaceCheck, " \t\n\v", &bufWhiteSpaceCheck);
	if ( strSansWhiteSpace == NULL ) {
		return 1;
	} else {
		return 0;
	}
}
