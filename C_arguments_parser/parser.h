#pragma once
#include <stdbool.h> // bool type
#include <stdlib.h>
#include <string.h>

struct flag {
    char* name;
    char* value;
    bool boolean_flag;
};
struct argument {
    char* name;
    char* value;
};

// ----------------- Functions -----------------
// string operations
static bool startswith(char* str, char* prefix);
static bool contains(char* str, char* substr);
static int charpos(char* str, char tofind);
// strdup implementations
static char* _strdup (const char* str);
static char* _strndup(const char* start, size_t n_bytes);

#ifndef MAX_FLAGS
#   define MAX_FLAGS 8
#endif
#ifndef MAX_ARGS
#   define MAX_ARGS 8
#endif

// ----------------- Global variables -----------------
struct flag g_program_flags[MAX_FLAGS];
struct flag g_program_args[MAX_ARGS];
int g_nprogram_flags = 0;
int g_nprogram_args = 0;

// ----------------- Definitions -----------------

/*
Parses and validates arguments.
Must be called before using `getarg()` or `isflagset()`.

When a parsing error occurs, the program exits with a nice error message and the 'Usage:' information.

Also, `freearguments()` must be called to free memory allocated by this function.
*/
void parseargs(char* usage, int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Usage: %s\n", usage);
        exit(-1);
    }
    // --- Usage information
    struct defined_flag {
        char* name;
        bool boolean_flag;
    };
    struct defined_flag accepted_flags[MAX_FLAGS]; // need to be freed
    int nflags = 0;
    char* required_args[MAX_FLAGS]; // also
    int nargs_required = 0;

    // --- Parsing usage
	char delim[] = " ";

    char* usage_cpy = _strdup(usage); // freed
	char *current = strtok(usage_cpy, delim); // first token
    // looping through all the tokens
	while ((current = strtok(NULL, delim)) != NULL) {
        // flags
        if (startswith(current, "-")) {
            // flag with value
            if (contains(current, "=")) {
                // get flag name
                int pos = charpos(current, '=');

                // same as strdup() but without the entire string
                accepted_flags[nflags].name = _strndup(current, pos);
                accepted_flags[nflags].boolean_flag = false;
            }
            // boolean flag
            else {
                // just save flag name
                accepted_flags[nflags].name = _strdup(current);
                accepted_flags[nflags].boolean_flag = true;
            }
            nflags++;
        }
        // arguments
        else if (startswith(current, "<")) {
            // printf("arg = %s\n", current);
            required_args[nargs_required] = _strdup(current);
            nargs_required++;
        }
	}
    free(usage_cpy);

    // --- Debugging
    // print accepted flags
    for (int i = 0; i < nflags; i++) {
        // printf(" > accepted flag = '%s'\n", accepted_flags[i].name);
    }
    // print required args
    for (int i = 0; i < nargs_required; i++) {
        // printf(" > required arg = '%s'\n", required_args[i]);
    }

    /* --- Parsing received arguments in char* argv[] --- */
    int nflags_received = 0;
    int nargs_received = 0;
    // looping through argv
    for (int i = 1/*skip bin name*/; i < argc; i++) {
        char* current_arg = argv[i];
        // checking if it's a --flag
        if (startswith(current_arg, "-")) {
            // check if it's a 'boolean flag' or 'value flag'
            if (contains(current_arg, "=")) {
                int pos = charpos(current_arg, '=');
                if (pos < 2 || pos == strlen(current_arg) - 1) {
                    fprintf(stderr, "malformed flag '%s'\n", current_arg);
                    fprintf(stderr, "Usage: %s\n", usage);
                    exit(-1);
                    return;
                }
                char* flag_name;
                char* flag_value;

                flag_name  = _strndup(current_arg, pos); // copy flag name
                flag_value = _strndup(
                    current_arg + (pos + 1),
                    strlen(current_arg) - (pos + 1)
                ); // copy flag value

                // printf("\nsaving name=%s value=%s\n\n", flag_name, flag_value);
                // save values.
                // (note that are memory allocated that will be freed at the end)
                g_program_flags[nflags_received].name = flag_name;
                g_program_flags[nflags_received].value = flag_value;
                g_program_flags[nflags_received].boolean_flag = false;
            }
            else {
                g_program_flags[nflags_received].name = _strdup(current_arg);
                g_program_flags[nflags_received].value = NULL; // boolean flag
                g_program_flags[nflags_received].boolean_flag = true;
            }
            // check if flag exists in Usage definition
            bool flag_exists = false;
            bool flag_valid  = false;
            struct flag* curr_flag = &g_program_flags[nflags_received];
            struct defined_flag* matched_flag = NULL;
            for (int i = 0; i < nflags; i++) {
                if (strcmp(accepted_flags[i].name, curr_flag->name) == 0) {
                    flag_exists = true;
                    matched_flag = &accepted_flags[i];
                    if (accepted_flags[i].boolean_flag == curr_flag->boolean_flag) {
                        flag_valid = true;
                    }
                    break;
                }
            }
            if (!flag_exists) {
                fprintf(stderr, "received unexpected flag '%s'\n", curr_flag->name);
                fprintf(stderr, "Usage: %s\n", usage);
                exit(-1);
                return;
            }
            if (!flag_valid) {
                // printf("value=%s, boolean=%d\n", curr_flag->value, curr_flag->boolean_flag);
                fprintf(
                    stderr,
                    curr_flag->value != NULL && matched_flag->boolean_flag
                        ? "flag '%s' does not expect a value\n"
                        : "flag '%s' expects a value\n",
                    curr_flag->name
                );
                fprintf(stderr, "Usage: %s\n", usage);
                exit(-1);
                return;
            }
            // printf("received valid flag: %s", curr_flag->name);
            if (!g_program_flags[nflags_received].boolean_flag) {
                // printf(" / value=%s", g_program_flags[nflags_received].value);
            }
            // putchar('\n');
            nflags_received++;
        }
        // is an <argument>
        else {
            if (nargs_received >= nargs_required) {
                fprintf(
                    stderr, "error: only %d arguments expected\n",
                    nargs_required
                );
                fprintf(stderr, "Usage: %s\n", usage);
                exit(-1);
            }
            // printf("received arg %s = %s\n", required_args[nargs_received], current_arg);
            g_program_args[nargs_received].name = _strdup(required_args[nargs_received]);
            g_program_args[nargs_received].value = _strdup(current_arg);
            nargs_received++;
        }
    }

    // free memory
    for (int i = 0; i < nflags; i++) {
        free(accepted_flags[i].name);
    }
    for (int i = 0; i < nargs_required; i++) {
        free(required_args[i]);
    }

    if (nargs_required != nargs_received) {
        fprintf(
            stderr,
            "error: received %d arguments, expected %d\n",
            nargs_received, nargs_required
        );
        fprintf(stderr, "Usage: %s\n", usage);
        exit(-1);
    }
    // save number of flags received
    g_nprogram_args = nargs_received;
    g_nprogram_flags = nflags_received;
}

/*
When called with a non-boolean flag name, it returns the value of the flag.
If the flag is a boolean flag, use `isflagset()` instead (this will throw an error).

When called with an argument name, it returns the value of the argument.

If the flag or argument doesn't exist, returns NULL.
*/
char* getarg(char* arg_name) {
    // get argument value
    if (startswith(arg_name, "<")) {
        for (int i = 0; i < g_nprogram_args; i++) {
            if (strcmp(g_program_args[i].name, arg_name) == 0) {
                return g_program_args[i].value;
            }
        }
        return NULL;
    }
    // get non-boolean flag value
    // if it's a boolean flag, use isflagset() instead
    else if (startswith(arg_name, "-")) {
        for (int i = 0; i < g_nprogram_flags; i++) {
            if (strcmp(g_program_flags[i].name, arg_name) == 0) {
                if (g_program_flags[i].boolean_flag) {
                    // If it's a boolean flag, it doesn't have a value
                    fprintf(
                        stderr,
                        "fatal error: attempt to get value of boolean flag '%s'\n", arg_name
                    );
                    exit(-1);
                    return NULL;
                }
                return g_program_flags[i].value;
            }
        }
        return NULL;
    }
    else {
        fprintf(
            stderr,
            "fatal error: in getarg(\"%s\")\nargument name must start with '-' or '<'\n",
            arg_name
        );
        exit(-1);
    }
    return NULL;
}

/*
Returns true if the flag is set, false otherwise (bool defined in <stdbool.h>).
Works for both boolean and non-boolean flags.
*/
bool isflagset(char* flag_name) {
    // get boolean flag value
    for (int i = 0; i < g_nprogram_flags; i++) {
        // printf("checking %s", g_program_flags[i].name);
        if (!g_program_flags[i].name) return false;
        // printf("comparing %s to %s\n", program_flags[i].name, flag_name);
        if (strcmp(g_program_flags[i].name, flag_name) == 0) {
            return true;
        }
    }
    return false;
}

/*
Free memory allocated in global variables by `parseargs()`.
After calling this function you will not be able to use `getarg()` or `isflagset()`.
*/
void freearguments() {
    for (int i = 0; i < g_nprogram_flags; i++) {
        free(g_program_flags[i].name);
    }
    for (int i = 0; i < g_nprogram_args; i++) {
        free(g_program_args[i].name);
    }
}

static bool startswith(char* str, char* prefix) {
    size_t prefix_len = strlen(prefix);
    char* buff = malloc(prefix_len + 1);
    memcpy(buff, str, prefix_len);
    buff[prefix_len] = '\0';

    bool result = strcmp(prefix, buff) == 0;
    free(buff);
    return result;
}

static char* _strndup(const char* start, size_t n_bytes) {
    if (n_bytes > strlen(start)) {
        fprintf(
            stderr,
            "fatal error: at my_strdup(), attempt to %ld bytes from string '%s'\n",
            n_bytes, start
        );
        exit(-1);
    }
    char* copy = malloc(n_bytes + 1);
    if (!copy) return NULL;

    memcpy(copy, start, n_bytes);
    copy[n_bytes] = '\0';
    return copy;
}

static char* _strdup (const char* str) {
    size_t len = strlen(str);
    char* allocated = malloc(len + 1);

    if (!allocated) return NULL;
    strcpy(allocated, str);

    return allocated;
}

static int charpos(char* str, char tofind) {
    return strchr(str, tofind) - str;
}

static bool contains(char* str, char* substr) {
    return strstr(str, substr) != NULL;
}
