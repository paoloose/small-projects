#pragma once
#include "icmp_structs.h"

#include <stdlib.h> // size_t

uint16_t calculate_checksum(icmp_ping_packet_t *buf) {
    uint16_t sum = 0; // sum to compute and take the one's complement of
    size_t packet_len = sizeof(icmp_ping_packet_t);
    // Read the buffer as 16 bit words (as ICMP protocol specifies)
    uint16_t* w = (uint16_t*)buf;

    while (packet_len > 1) {
        sum += *w++; // sum and increment the pointer to the next 16 bit word
        packet_len -= sizeof(uint16_t);
    }

    // If there is a single byte left, pad the byte and add it to the sum
    if (packet_len == 1) {
        sum += *(uint8_t *)w; // cast to 8 bit (1 byte)
    }

    // If we shift our sum to 16 bits and it is not 0, then we have a carry
    uint16_t carry;
    while ((carry = sum) >> 16) {
        // while there is a carry, add the carry to the sum
        // and shift the sum to 16 bits again
        sum = (sum & 0xffff) + carry; // 0xffff is 16 bits of 1's
    }

    return ~sum; // return the one's complement of sum
}
