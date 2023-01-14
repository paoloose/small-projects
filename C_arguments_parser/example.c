#include <stdio.h>
#include <string.h>
#include "parser.h"

char usage[] = "my_program --ipv4 --ipv6 --ttl=TTL <host> <port>";

int main(int argc, char *argv[]) {
    parseargs(usage, argc, argv);

    char* host = getarg("<host>");
    char* port = getarg("<port>");

    printf("argument: <host>: %s\n", host);
    printf("argument: <port>: %s\n", port);

    if (isflagset("--ipv4")) {
        printf("'--ipv4' flag is set\n");
    }
    if (isflagset("--ipv6")) {
        printf("'--ipv6' flag is set\n");
    }
    if (isflagset("--ttl")) {
        printf("'--ttl' flag is set (value: %s)\n", getarg("--ttl"));
    }
    getarg("--non-existing"); // returns NULL
    isflagset("--non-existing"); // returns false (0)

    // getarg("--ipv4");
    // exit with error message
    // "fatal error: attempt to get value of boolean flag --ipv4"

    freearguments(); // free memory allocated by parseargs()
    return 0;
}

// For example, run:
// gcc -Wall -o program example.c && ./program --ipv4 --ttl=10 192.168.1.1 443 --ipv6
