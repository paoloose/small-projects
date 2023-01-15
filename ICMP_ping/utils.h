#pragma once

#include <stdlib.h> // exit
#include <stdio.h> // perror
#include <unistd.h> // getpid()
#include <stdint.h> // uint8_t, uint16_t, uint32_t

#include <errno.h> // error handling

#include <arpa/inet.h> // inet_addr()

char* parse_args(int argc, char* argv[]) {

    if (argc < 2) {
        // set errno value
        errno = EINVAL;

        perror("bad input");
        exit(EXIT_FAILURE);
        return NULL;
    }
    return argv[1];
}

// Unique identifier for the packet
// process id is used so we can have multiple instances running
pid_t get_packet_unique_id() {
    return getpid();
}

// Convert the ip address to bytes
// remember that ip addresses are just a 32-bit number (IPv4)
uint32_t convert_ip_to_bytes(char* address) {
    return inet_addr(address);
}
