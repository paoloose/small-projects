
# C argument parser

Dummy _weekend_ projects.

Simple C argument parser for simple C programs.

## Usage

This argument parser is designed to be used in simple C CLI programs.

Just include the `parser.h` and you are ready to go.

Define your flags and arguments just with a single string!!.

```c
#include <stdio.h>
#include <string.h>
#include "parser.h"

// Define usage string for your program.
// Example with a ping program:
char usage[] = "my_program --ipv4 --ipv6 --ttl=TTL <host> <port>";

int main(int argc, char *argv[]) {
    parseargs(usage, argc, argv); // load arguments and check if they are valid

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

    freearguments(); // free memory
    return 0;
}
```

Now you can compile and run your program with your defined arguments.

We handle the following cases:

```bash
# Valid cases

./my_program --ipv4 --ttl=10 google.com 80 # valid
./my_program google.com 80 # no flags, valid
./my_program --ipv4 --ipv6 google.com 80 # semantically invalid, but valid

# Invalid cases

./my_program
# Usage: my_program --ipv4 --ipv6 --ttl=TTL <host> <port>

./my_program google.com 80 443
# error: only 2 arguments expected

./my_program google.com
# error: received 1 arguments, expected 2

./my_program --ipv4 --ttl= google.com 80
# error: malformed flag: '--ttl='

./my_program --ipv4=true google.com 443
# error: flag '--ipv4' does not expect a value

./my_program --ttl google.com 80
# error: flag '--ttl' expects a value

./my_program --ipv4 --verbose google.com 80
# error: received unexpected flag: '--verbose'
```

Yes, we handle errors for you and print them to `stderr`.

## Limitations

- All flags are **optional** by default and by design.
- All arguments are **required** by default and by design.
- Complex syntax like required flags or optional arguments are not supported.

Features may be added in the future as needed.

Inspired by [docopt](http://docopt.org/).
