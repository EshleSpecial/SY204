/*
 * Name: MIDN McKenzie Eshleman
 * Alpha: 221938
 * Course: SY204 - Sys Prgm & OS Fund
 * Assignment: Untouchables
 *
 * Description: Modify regular file but do not change chance access time or modify time
 *
 * Resources: UNIX Manual
 *
 * NAME
 *		untouch - Modify regular file but do not change access time or
 *		          modify time
 * SYNOPSIS
 *		untouch FILE
 * DESCRIPTION
 *		Modify FILE (regular file) but do not change the last accessed and
 *		last modified times. If FILE does not exist, create FILE assigning
 *	last accessed and last modified times of the parent directory.
 *
 *		If the command line argument is not a regular file, no actions are
 *		taken, and the program exits normally.
 */

/*
 * Uncomment below line to use split second, then nanosecond solution.
 * Default solution uses combined second and nanosecond solution.
 */
//#define SPLIT_SEC_NANO

// Required Includes
// Previous Uses: #include <libname.h>  // <Library Name>: <Uses: constants, variables, system calls, library routines>...
// #include <unistd.h>  // UNIX Standard Library: close(2), read(2), write(2)
#include <errno.h>      // Error Number Library: E*, errno
#include <libgen.h>     // General Library: basename(3), dirname(3)
#include <stddef.h>     // Standard Definitions Library:
#include <stdio.h>      // Standard I/O Library: fprintf(3)
#include <stdlib.h>     // Standard Library:
#include <unistd.h>     // UNIX Standard Library:
#include <fcntl.h>      // File Descriptor Control Library:
#include <string.h>     // String Library: strncpy(3)
#include <utime.h>      // Time operations: struct utimbuf, utime(2)
#include <sys/stat.h>   // System File System Library: stat(2)
#include <sys/types.h>  // System Types Library: various defined data types, various defined constants
#include <sys/time.h>   // System Time Lbrary: struct timeval, utimes(2)
#include <getopt.h>     // Get Options Library: optarg, opterr, optind, optopt, getopt(3)

// Global Constants and Variables
#define MAX_STR_LEN 1024
char strBaseName[MAX_STR_LEN];   // String for program name
char strBaseError[MAX_STR_LEN];  // String for base error message
char strErrorMsg[MAX_STR_LEN];   // String for error message

// Structure and Type Definitions

// Function Prototypes
int setupErrorMsg(const char *strCalledName);
void exitErrorMsg(int intIntrnErrNum, int intErrNum, const char *strIntrnErrNum, const char *strErrMsgReport);
int reportErrorMsg(int intErrNum, const char *strIntrnErrNum, const char *strErrMsgReport);
void usage(void);
int saveStatTimes(struct stat stEntity, struct timespec *tmspTimes);

int main( int argc, char *argv[] ) {
	/*
	 * Description: Main program control flow and logic
	 * Args:
	 *   argv[1] : Name of file to modify, but retain original time stamps
	 * Return:
	 *   0 : Normal return
	 *   ...
	 */
	// Declare Required Variables
	char chrOption = 0;              // Option
	int intReturnStatus = 0;         // Assume normal exit
	int intFailure = -1;             // Integer to check return values
	int intDataIO = -1;              // Number of bytes from IO operation
	int fdTarget = -1;               // File descriptor
	int argTgt = 1;                  // Target argument
	off_t ofstTarget = -1;           // File descriptor offset
	struct stat stTarget = { 0 };    // Stat to store stat information, {0}: nils fields
	struct timespec *tmspBefore = NULL;  // Time Value to store time information
	char strInject[7] = "-h0ff\n";   // String to append to file
	char strFullPath[MAX_STR_LEN];   // String for full path
	char *strParentDir;              // String for parent directory
	// Perform function
	intFailure = setupErrorMsg(argv[0]);  // Setup error reporting
	if ( intFailure ) {
		fprintf(stderr, "%s-CAUTION: Unable to setup error messages. [continuing]\n", argv[0]);
	}
	if ( argc <= 1 ) {
		usage();
		return 0;
	}
	opterr = 0;  // Tell getopt not to report errors
	while ( (chrOption=getopt(argc, argv, "h")) != -1 ) {
		switch (chrOption) {
			case 'h':
				usage();
				return 0;
				break;
			default:
				reportErrorMsg(ENOSYS, "38-", "Unsupported Option");
				break;
		}
	}
	argTgt = optind;  // Next command line argument (read in all options)
	intFailure = stat(argv[argTgt], &stTarget);
	if ( intFailure ) {  // Error, unable to stat FILE
		switch (errno) {
			case ENOENT :  // Recover, FILE does not exist, but can
				strncpy(strFullPath, argv[argTgt], MAX_STR_LEN);
				strFullPath[MAX_STR_LEN - 1] = '\0';
				strParentDir = dirname(strFullPath);
				intFailure = stat(strParentDir, &stTarget);
				if ( intFailure ) {
					exitErrorMsg(8, errno, "8-", strParentDir);
				}
				fdTarget = open(argv[argTgt], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
				if ( fdTarget == -1 ) {
					exitErrorMsg(9, errno, "9-", argv[1]);
				}
				break;
			default :  // Fatal Error
				exitErrorMsg(10, errno, "10-", argv[1]);
		}
	} else {  // Target exists, able to stat FILE
		if ( ! S_ISREG(stTarget.st_mode) ) {  // Argument is not a regular file
			exitErrorMsg(1, EINVAL, "1-", argv[argTgt]);
		}
		// Setup file for modification
		fdTarget = open(argv[argTgt], O_WRONLY);
		if ( fdTarget == -1 ) {
			exitErrorMsg(2, errno, "2-", argv[1]);
		}
		ofstTarget = lseek(fdTarget, 0, SEEK_END);
		if ( ofstTarget == -1 ) {
			exitErrorMsg(3, errno, "3-", argv[1]);
		}
	}
	// Common save original access and modified times
	tmspBefore = calloc(2, sizeof(struct timespec));
	if ( tmspBefore == NULL ) {
		exitErrorMsg(12, errno, "12-", "calloc for timespec failed");
	}
	intFailure = saveStatTimes(stTarget, tmspBefore);
	if ( intFailure ) {
		exitErrorMsg(4, ENOTRECOVERABLE, "4-", "stat to tmspBefore failed");
	}
	// Common modify file
	intDataIO = write(fdTarget, strInject, 6);
	if ( intDataIO == -1 ) {
		exitErrorMsg(5, errno, "5-", argv[argTgt]);
	}
	intFailure = futimens(fdTarget, tmspBefore);
	if ( intFailure ) {
		exitErrorMsg(6, errno, "6-", argv[argTgt]);
	}
	intFailure = close(fdTarget);
	if ( intFailure ) {
		exitErrorMsg(7, errno, "7-", argv[argTgt]);
	}
	// Memory management
	free(tmspBefore);
	// Return to caller
	return intReturnStatus;
}

// Setups the global error message variables
int setupErrorMsg(const char *strCalledName) {
	// Declare required variables
	int intReturnStatus = 0;    // Assume normal exit
	char *strBaseNamed;          // String for program base name
	// Perform function
	strncpy(strBaseName, strCalledName, MAX_STR_LEN - 1);
	strBaseName[MAX_STR_LEN - 1] = '\0';
	strBaseNamed = basename(strBaseName);  // Only this file name feature (we're all friends here, right?)
	strncpy(strBaseName, strBaseNamed, MAX_STR_LEN - 1);
	strBaseName[MAX_STR_LEN - 1] = '\0';
	if ( (strlen(strBaseError) + strlen(strBaseName)) < MAX_STR_LEN ) {
		strcat(strBaseError, strBaseName);
	} else {
		fprintf(stderr, "%s-CAUTION: Long base name detected, error messages may be truncated. [continuing]\n", strBaseName);
		intReturnStatus = -1;
	}
	strBaseError[MAX_STR_LEN - 1] = '\0';  // Ensure string is terminated
	if ( (strlen(strBaseError) + strlen("-ERROR ")) < MAX_STR_LEN ) {
		strcat(strBaseError, "-ERROR ");
	} else {
		fprintf(stderr, "%s-CAUTION: Long base name detected, error messages may be truncated. [continuing]\n", strBaseName);
		intReturnStatus = -1;
	}
	strBaseError[MAX_STR_LEN - 1] = '\0';  // Ensure string is terminated
	return intReturnStatus;
}

// Reports the error message specified and exits
void exitErrorMsg(int intIntrnErrNum, int intErrNum, const char *strIntrnErrNum, const char *strErrMsgReport) {
	// Declare required variables
	// Perform function
	reportErrorMsg(intErrNum, strIntrnErrNum, strErrMsgReport);
	exit(intIntrnErrNum);
}

// Reports the error message specified
int reportErrorMsg(int intErrNum, const char *strIntrnErrNum, const char *strErrMsgReport) {
	// Declare required variables
	int intReturnStatus = 0;  // Assume normal exit
	// Preform function
	strncpy(strErrorMsg, strBaseError, MAX_STR_LEN);
	strErrorMsg[MAX_STR_LEN - 1 ]='\0';
	strcat(strErrorMsg, strIntrnErrNum);
	strErrorMsg[MAX_STR_LEN - 1 ]='\0';
	strcat(strErrorMsg, strErrMsgReport);
	strErrorMsg[MAX_STR_LEN - 1 ]='\0';
	errno = intErrNum;
	perror(strErrorMsg);
	return intReturnStatus;
}

// Outputs usage information
void usage(void) {
	// Declare required variables
	// Perform function
	fprintf(stdout, "NAME\n");
	fprintf(stdout, "\t%s\t- Modify regular file but do not change access time or\n", strBaseName);
	fprintf(stdout, "\t\t  modify time\n\n");
	fprintf(stdout, "SYNOPSIS\n");
	fprintf(stdout, "\t%s <FILE>\n\n", strBaseName);
	fprintf(stdout, "DESCRIPTION\n");
	fprintf(stdout, "\tModify FILE (regular file) but do not change the last accessed or\n");
	fprintf(stdout, "\tlast modified times. If FILE does not exist, create FILE assigning\n");
	fprintf(stdout, "\tlast accessed and last modified times of the parent directory.\n\n");
	fprintf(stdout, "\tIf the command line argument is not a regular file, no actions are\n");
	fprintf(stdout, "\ttaken, and the program exits with an error.\n\n");
	fprintf(stdout, "\t-h\tdisplay this help.\n");
	return ;
}

// Save the access and modified times from stEntity to utmTimes adn tmspTimes
int saveStatTimes(struct stat stEntity, struct timespec *tmspTimes) {
	// Declare required variables
	int intReturnStatus = -1;
	// Perform function
	#ifdef SPLIT_SEC_NANO
		// Separate Part 1, then Part 2 Solution
		tmspTimes[0].tv_sec = stEntity.st_atime;
		tmspTimes[0].tv_nsec = stEntity.st_atim.tv_nsec;
		tmspTimes[1].tv_sec = stEntity.st_mtime;
		tmspTimes[1].tv_nsec = stEntity.st_mtim.tv_nsec;
	#else
		// Combined Part 1 and Part 2 Solution
		tmspTimes[0]=stEntity.st_atim;
		tmspTimes[1]=stEntity.st_mtim;
	#endif
	intReturnStatus = 0;
	return intReturnStatus;
}
