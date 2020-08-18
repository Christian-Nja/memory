#include <sqlite3.h>

#define COMMAND_NOT_FOUND -1

/**
 * Create tables for local sqlite db
 * */
int create_db_tables();
/**
 * Inserts COMMAND into local db if not exists
 * returns row ID of the COMMAND 
 **/
int insert_command(char *command);

/**
 * Inserts a usage EXAMPLE for the command with id COMMAND_ID
 * and a COMMENT, returns EXIT_SUCCESS
 **/
int insert_example(char *example, char *comment, int command_id);

/**
 * Selects all usage example for a COMMAND
 * */
int select_all_examples(char *command);

/**
 * Returns ordered list of memorized commands 
 * */
int select_all_commands();

/**
 * In the main this function begins the logic to add a new command into memory commands db.
 * It prompts user for a command, example and comment
 * */
int new_command(int argc, char *command);

/**
 * In the main this functions starts the logic to initialize the database for commands
 **/
int initialize_db();

/**
 * Show informations about a COMMAND
 * */
int show(int argc, char *command);

/**
 * Delete a COMMAND and all related examples or delete example by ID for given COMMAND
 * */
int cancel(int argc, char *command, int example_id);
