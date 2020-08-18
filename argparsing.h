#include <string.h>
#include <argp.h>
#include <memoryConfig.h>
/***
 ***    ARGUMENTS           used by main to communicate
 ***                        with parse_opt
 ***/
struct arguments
{
    char *args[1]; /* db | new | show <command> */
};

/*******
 *
 *          GLOBALS 
 * 
 *******/

/***   
 ***    VERSION             print by --version
 ***/
// char program_version[13];
// strcpy(program_version, "memory ");
// strcat(program_version, memory_VERSION_MAJOR);
// strcat(program_version, ".");
// strcat(program_version, memory_VERSION_MINOR);

const char *argp_program_version =
    "memory 1.0";

/***   
 ***    BUG ADDRESS         print by --help
 ***/
const char *argp_program_bug_address =
    "<noemail@protonmail.com>";

/***
 ***    DOCUMENTATION       print by --help
 ***/
const char doc[] =
    "A program to memorize command usage examples and to fast retrieve information about a command";

/***
 ***    ARGS DOC            a description for the argument accepted
 ***/
static char args_doc[] = "db\t\t\t\tinitialize the db in ~/.memory folder\n"
                         "new <COMMAND>\t\t\tmemorize an example for the command\n"
                         "show\t\t\t\tshow all the commands\n"
                         "show <COMMAND>\t\tshow usage examples for the command";

/***
 ***    OPTIONS             doc for option
 ***                        print by --help
 ***/
static struct argp_option options[] = {
    {0}};

/***
 ***    OPTION PARSING FUNCTION
 ***/
static error_t parse_opt(int key, char *arg, struct argp_state *state);
