/**
 * Functions to work with db
 * */

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include "db.h"

/*** PATH TO DB ***/
const char *DB_PATH = "/home/linux/.memory/commands.db";

/* global definition for the DB to be used all 
   over the functions */
sqlite3 *DB;

/**
 * Open connection to globally declared DB at DB_PATH
 * */
int open_db_connection()
{
    // open connection
    int rc = sqlite3_open(DB_PATH, &DB);
    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(DB));
        sqlite3_close(DB);

        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

/**
 * Raises error and closes globally declared DB
 * */
int exec_db_error(char *err_msg)
{

    fprintf(stderr, "SQL error [commands]: %s\n", err_msg);

    sqlite3_free(err_msg);
    sqlite3_close(DB);

    exit(EXIT_FAILURE);
}

/**
 * Raises error and closes globally declared DB
 * */
int prepare_db_error()
{
    fprintf(stderr, "%s: %s\n", "Failure fetching data: ", sqlite3_errmsg(DB));
    sqlite3_close(DB);
    exit(EXIT_FAILURE);
}

/**
 * Raises error, finalizes STATEMENT and closes globally declared DB 
 * */
int stmt_db_error(sqlite3_stmt *stmt)
{
    printf("Execution failed: %s", sqlite3_errmsg(DB));
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    exit(EXIT_FAILURE);
}

/**
 * Create tables for local sqlite db
 * */
int create_db_tables()
{
    char *err_msg;

    open_db_connection();

    // create table query
    char *sql_commands_and_examples = "CREATE TABLE IF NOT EXISTS commands "
                                      "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      " name TEXT NOT NULL UNIQUE);"
                                      ""
                                      "CREATE TABLE IF NOT EXISTS examples  "
                                      "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      " example TEXT NOT NULL,"
                                      " comment TEXT, "
                                      " command_id INTEGER NOT NULL, "
                                      " FOREIGN KEY (command_id) REFERENCES commands(id) );";

    // create commands
    int rc = sqlite3_exec(DB, sql_commands_and_examples, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        exec_db_error(err_msg);
    }

    sqlite3_close(DB);

    return EXIT_SUCCESS;
}

/**
 * Returns command id for COMMAND
 * connection to the db must be already open
 * */
int get_command_id_by_name(char *command)
{
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, "SELECT id FROM commands WHERE name=?1 ;", -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        prepare_db_error();
    }

    //bind values to parameters
    sqlite3_bind_text(stmt, 1, command, -1, SQLITE_STATIC);

    //run the SQL
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        // if command already exists returns row id
        int command_id = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);

        return command_id;
    }
    else
    {
        return COMMAND_NOT_FOUND;
    }
}

/**
 * Inserts COMMAND into local db if not exists
 * returns row ID of the COMMAND 
 **/
int insert_command(char *command)
{
    int command_id;
    // open connection
    open_db_connection();

    // try to get the command id from the db commands table if it already exists
    command_id = get_command_id_by_name(command);

    if (command_id != COMMAND_NOT_FOUND)
    {
        //close db
        sqlite3_close(DB);
        return command_id;
    }

    // create commands fetch the id and returns it
    char *err_msg = 0;
    char *sql_command = sqlite3_mprintf("INSERT INTO commands(name) VALUES ('%q');", command);

    // create command
    int rc = sqlite3_exec(DB, sql_command, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        exec_db_error(err_msg);
    }
    int last_id = sqlite3_last_insert_rowid(DB);
    sqlite3_close(DB);

    return last_id;
}

/**
 * Inserts a usage EXAMPLE for the command with id COMMAND_ID
 * and a COMMENT, returns EXIT_SUCCESS
 **/
int insert_example(char *example, char *comment, int command_id)
{
    // open connection
    open_db_connection();

    // prepare stmt
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, "INSERT INTO examples (example, comment, command_id) VALUES (?1, ?2, ?3) ;", -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        prepare_db_error();
    }

    //bind values to parameters
    sqlite3_bind_text(stmt, 1, example, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, comment, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, command_id);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        stmt_db_error(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return EXIT_SUCCESS;
}

/**
 * Selects all usage example for a COMMAND
 * */
int select_all_examples(char *command)
{

    // open connection
    open_db_connection();

    sqlite3_stmt *stmt;
    char *sql_command = "SELECT id, example, comment    "
                        "FROM examples WHERE command_id "
                        "IN "
                        "  (  SELECT id FROM commands   "
                        "     WHERE name = (?)     );   ";

    int rc = sqlite3_prepare_v2(DB, sql_command, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        prepare_db_error();
    }

    sqlite3_bind_text(stmt, 1, command, -1, SQLITE_STATIC);

    printf("\n %s \n\n", command);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *example = sqlite3_column_text(stmt, 1);
        const unsigned char *comment = sqlite3_column_text(stmt, 2);
        printf(" %d) Usage: %s\n\n    Comment: %s\t\t\n", id, example, comment);
        printf("\n*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=\n\n");
    }

    if (rc != SQLITE_DONE)
    {
        printf("error: %s", sqlite3_errmsg(DB));
    }
    sqlite3_finalize(stmt);

    sqlite3_close(DB);
    return EXIT_SUCCESS;
}

/**
 * Returns ordered list of memorized commands 
 * */
int select_all_commands()
{
    open_db_connection();

    // prepare stmt
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, "SELECT name FROM commands ORDER BY name ASC;", -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        prepare_db_error();
    }

    // run sql and iterate through results
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char *command = sqlite3_column_text(stmt, 0);
        printf("%s\t\t\n", command);
    }

    if (rc != SQLITE_DONE)
    {
        printf("error: %s", sqlite3_errmsg(DB));
    }
    sqlite3_finalize(stmt);

    sqlite3_close(DB);
    return EXIT_SUCCESS;
}

/**
 * Deletes COMMAND and all examples for the given command
 * */
int delete_command(char *command)
{
    int command_id;

    open_db_connection();

    command_id = get_command_id_by_name(command);
    if (command_id == COMMAND_NOT_FOUND)
    {
        printf("[!] There's no command for given name: %s\n", command);
        sqlite3_close(DB);
        return EXIT_FAILURE;
    }

    char *err_msg = 0;
    char *sql_command = sqlite3_mprintf("BEGIN TRANSACTION; "
                                        "DELETE FROM examples WHERE command_id = %d ;"
                                        "DELETE FROM commands WHERE name = '%s';"
                                        "COMMIT;",
                                        command_id, command);

    // create command
    int rc = sqlite3_exec(DB, sql_command, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        exec_db_error(err_msg);
    }
    sqlite3_close(DB);
    printf("[*] Succesfully deleted: %s\n", command);
    return EXIT_SUCCESS;
}

/**
 * Deletes example with EXAMPLE_ID for COMMAND
 * */
int delete_command_example(char *command, int example_id)
{
    open_db_connection();

    int command_id = get_command_id_by_name(command);
    if (command_id == COMMAND_NOT_FOUND)
    {
        printf("[!] There's no command for given name: %s\n", command);
        sqlite3_close(DB);
        return EXIT_FAILURE;
    }

    char *err_msg = 0;
    char *sql_command = sqlite3_mprintf("DELETE FROM examples WHERE id = %d AND command_id = %d;", example_id, command_id);

    // create command
    int rc = sqlite3_exec(DB, sql_command, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        exec_db_error(err_msg);
    }
    int last_id = sqlite3_last_insert_rowid(DB);
    printf("[*] Example n_%d succesfully deleted for command: %s\n", example_id, command);
    sqlite3_close(DB);
}

/**
 * In the main this function starts the logic to add a new command into memory commands db.
 * It prompts user for a command, example and comment
 * */
int new_command(int argc, char *command)
{
    char example[200];
    char comment[200];

    int command_id = insert_command(command);

    printf("Command line usage example: ");
    gets(example);

    printf("Comment: ");
    gets(comment);

    return insert_example(example, comment, command_id);
}

/**
 * Show informations about a COMMAND
 * */
int show(int argc, char *command)
{
    if (argc == 2)
    {
        return select_all_commands();
    }
    return select_all_examples(command);
}

/**
 * Delete a COMMAND and all related examples or delete example by ID for given COMMAND
 * */
int cancel(int argc, char *command, int example_id)
{
    if (argc == 2)
    {
        printf("[*] Specify a COMMAND to delete and optionally a -i ID for the example\n");
        return EXIT_SUCCESS;
    }
    if (example_id == -1)
    {
        printf("[*] Deleting command and all examples: %s\n", command);
        delete_command(command);
        return EXIT_SUCCESS;
    }
    else
    {
        printf("[*] Deleting example number %d for command %s\n", example_id, command);
        delete_command_example(command, example_id);
        return EXIT_SUCCESS;
    }
}

/**
 * In the main this functions starts the logic to initialize the database for commands
 **/
int initialize_db()
{
    create_db_tables();
    return EXIT_SUCCESS;
}
