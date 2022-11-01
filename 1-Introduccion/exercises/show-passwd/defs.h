#ifndef DEFS_H
#define DEFS_H

#define MAX_LOGIN_NAME  50
#define MAX_PASSWD_LINE  255

/**
 * Structure that describes an entry in /etc/passwd
 * Check "man 5 passwd" for details
 */
typedef struct passwd_entry {
	char login_name[MAX_LOGIN_NAME];
	char* optional_encrypted_passwd;
	int uid;
	int gid;
	char* user_name;
	char* user_home;
	char* user_shell;
} passwd_entry_t;

/**
 * Index of the various fields in passwd_entry_t
 * Helper data type to simplify the parser's implementation.
 */
typedef enum {
	LOGIN_NAME_IDX=0,    // ayhon
	ENCRYPTED_PASS_IDX,  // x
	UID_IDX,             // 1000
	GID_IDX,             // 1000
	USER_NAME_IDX,       // Ayhon
	USER_HOME_IDX,       // /home/ayhon
	USER_SHELL_IDX,		 // /bin/bash
	NR_FIELDS_PASSWD	 // 
} token_id_t;
// ayhon:x:1000:1000:Ayhon:/home/ayhon:/bin/bash

/**
 * Output modes supported by the program
 */
typedef enum {
	VERBOSE_MODE,
	PIPE_MODE,
	CSV_MODE
} output_mode_t;

/**
 * Structure to hold the "variables" associated with
 * command-line options
 */
struct options {
	FILE* outfile;
	char* infilename;
	output_mode_t output_mode;
};

#endif
