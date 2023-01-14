#include <stdio.h>
#include <string.h>
#include "parser.h"

char ping_usage[] = "my_program --ipv4 --ipv6 --ttl=TTL <host> <port>";

int main(int argc, char *argv[]) {
    usage(ping_usage, argc, argv);

    char* host = getarg("<host>");
    char* port = getarg("<port>");

    printf("get <host>: %s\n", host);
    printf("get <port>: %s\n", port);

    if (isflagset("--ipv4")) {
        printf("--ipv4 flag is set\n");
    }
    if (isflagset("--ipv6")) {
        printf("--ipv6 flag is set\n");
    }
    if (isflagset("--ttl")) {
        printf("--ttl flag is set\n");
        printf("--ttl value: %s\n", getarg("--ttl"));
    }
    getarg("--non-existing"); // returns NULL
    isflagset("--non-existing"); // returns false (0)

    return 0;
}
