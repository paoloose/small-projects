#include <stdlib.h> // exit
#include <stdio.h>  // printing

// Networking
#include <sys/socket.h> // socket API
#include <netinet/in.h>

#include "icmp_structs.h"
#include "icmp_checksum.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    
    char* ip_address = parse_args(argc, argv);

    printf("Pinging %s\n", ip_address);

    /* Standard well-defined IP protocols.  */
    // TODO: document these
    // Create socket and save the file descriptor
    int icmp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (icmp_socket <= 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Since socket is created, we need to define the icmp packet
    icmp_ping_packet_t packet_to_send;
    packet_to_send.type = ICMP_ECHO; // expands to 8
    packet_to_send.code = 0;
    packet_to_send.checksum = 0; // needs to be calculated
    packet_to_send.identifier = get_packet_unique_id();
    packet_to_send.sequence_number = 1; // first (and unique)
    packet_to_send.payload = 99;

    packet_to_send.checksum = calculate_checksum(&packet_to_send);
    printf("checksum=%d\n", packet_to_send.checksum);

    // We created a datagram packet, but we need to defined a socket address
    // to send that packet to
    struct sockaddr_in address_to_ping;
    // socket address = ip_address + port number
    address_to_ping.sin_family = AF_INET;
    address_to_ping.sin_port = 0; // not needed for ICMP
    address_to_ping.sin_addr.s_addr = convert_ip_to_bytes(ip_address);

    // Send the packet
    int bytes_sent = sendto(
        icmp_socket, // file descriptor of our socket
        &packet_to_send, sizeof(packet_to_send),
        0x0, // no flags needed
        (struct sockaddr*)&address_to_ping, sizeof(address_to_ping)
        // (struct sockaddr*)&address_to_ping, sizeof(address_to_ping)
    );

    if (bytes_sent < 0) {
        perror("error sending the ICMP datagram");
        exit(-1);
    }

    printf("Sent packet with identifier %d\n", packet_to_send.identifier);

    // Packet was sent, now we need to wait for the response...
    // Well... actually we are not waiting for "the response", we are waiting
    // for any packet that belong to us.
    // How can we know if the response is for the ping?
    // that is why the "identifier" field exists for
    unsigned char response_buffer[30] = {0};
    // Note that this isn't a struct anymore, but a buffer,
    // how to read from this buffer? we will see later
    struct sockaddr response_address;
    unsigned int response_address_size;

    icmp_ping_packet_t* response_packet;
    do {
        int bytes_received = recvfrom(
            icmp_socket,
            response_buffer, sizeof(response_buffer),
            0,
            &response_address, // is the address that we pinged to...?
            &response_address_size
        );

        if (bytes_received <= 0) {
            perror("error receiving a package");
            exit(-1);
        }

        // Here we received a packet, all good, but is it the response to our ping?
        // We need to check the identifier field.
        // Let's cast the buffer to a struct.

        // Response packet has both ip header and ICMP header
        // We need to skip the ip header to get to the ICMP header
        // ip header is 20 bytes long
        response_packet = (icmp_ping_packet_t*)(response_buffer + 20);

        printf("Received packet with identifier %d",response_packet->identifier);
        printf(" type=%d", response_packet->type);
        printf(" code=%d", response_packet->code);
        printf(" from %s\n", inet_ntoa(((struct sockaddr_in*)&response_address)->sin_addr));

    } while (response_packet->identifier != packet_to_send.identifier);

    return 0;
}
