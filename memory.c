/**
 * Program writes & reads notes from file .note
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <argp.h>

#include "memory.h"
#include "argparsing.h"
#include "db.h"

/*** commands ***/
const char *DBI = "db";
const char *NEW = "new";
const char *SHOW = "show";
const char *DELETE = "delete";

int main(int argc, char *argv[])
{
    struct arguments arguments;

    /***
     ***    ARGP PARSER
     ***/
    static struct argp argp = {options, parse_opt, args_doc, doc};

    /**
     * Parsing Arguments 
     ***/
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (strcmp(arguments.args[0], DBI) == 0)
    {
        return initialize_db();
    }
    else if (strcmp(arguments.args[0], NEW) == 0)
    {
        // gets the command from input if not specified
        char command[30];
        if (argc == 2)
        {
            printf("Command name: ");
            gets(command);
            return new_command(argc, command);
        }
        else
        {
            return new_command(argc, arguments.args[1]);
        }
    }
    else if (strcmp(arguments.args[0], SHOW) == 0)
    {
        return show(argc, arguments.args[1]);
    }
    else
    {
        printf("Invalid arguments\n");
    }

    /* switch with string (arguments.args[0]) 
       https://stackoverflow.com/a/37121071/12506641 
    */
}

/***
 ***    OPTION PARSING FUNCTION
 ***/
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    /* get input argument from argp_parse, a pointer to arguments structure
    */
    struct arguments *arguments = state->input;
    switch (key)
    {

    case ARGP_KEY_ARG:
        if (state->arg_num > 1)
            /* Too many arguments. */
            argp_usage(state);
        arguments->args[state->arg_num] = arg;

        break;

    case ARGP_KEY_END:
        if (state->arg_num < 1)
            /* Not enough arguments. */
            argp_usage(state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
