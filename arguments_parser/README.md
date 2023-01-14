
# C argument parser

Dummy _weekend_ projects
Simple C argument parser for simple C programs.

Example:

```c
#include <stdio.h>
#include <string.h>
#include "parser.h"

// Define usage string
char ping_usage[] = "my_program --ipv4 --ipv6 --ttl=TTL <host> <port>";

int main(int argc, char *argv[]) {
    usage(ping_usage, argc, argv); // load arguments and check if they are valid

    // all arguments are validated and loaded

    // get arguments
    char* host = getarg("<host>");
    char* port = getarg("<port>");

    // get flags
    if (isflagset("--ipv4")) {
        // do stuff
    }
    if (isflagset("--ipv6")) {
        // do stuff
    }
    if (isflagset("--ttl")) {
        char* ttl = getarg("--ttl");
        // do stuff
    }
    getarg("--non-existing"); // returns NULL
    isflagset("--non-existing"); // returns false (0)

    return 0;
}
```
