#pragma once
#include <stdint.h> // uint8_t, uint16_t, uint32_t

// Codes for ICMP messages
// Yes, ICMP is not only for pinging...
// Note: these types are defined in header <netinet/ip_icmp.h>
#define ICMP_ECHOREPLY		0	/* Echo Reply			    */
#define ICMP_DEST_UNREACH	3	/* Destination Unreachable	*/
#define ICMP_SOURCE_QUENCH	4	/* Source Quench		    */
#define ICMP_REDIRECT		5	/* Redirect (change route)	*/
#define ICMP_ECHO		    8	/* Echo Request			    */
#define ICMP_TIME_EXCEEDED	11	/* Time Exceeded		    */
#define ICMP_PARAMETERPROB	12	/* Parameter Problem		*/
#define ICMP_TIMESTAMP		13	/* Timestamp Request		*/
#define ICMP_TIMESTAMPREPLY	14	/* Timestamp Reply		    */
#define ICMP_INFO_REQUEST	15	/* Information Request		*/
#define ICMP_INFO_REPLY		16	/* Information Reply		*/
#define ICMP_ADDRESS		17	/* Address Mask Request		*/
#define ICMP_ADDRESSREPLY	18	/* Address Mask Reply		*/
#define NR_ICMP_TYPES		18

typedef struct {
    /* ICMP header */
    // For the following two fields see:
    // https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol#Control_messages
    uint8_t type; // message type, ECHO TYPE IS 8
    uint8_t code; // sub-code of the type
    // Checksum is an error-check field, and needs to be calculated based on
    // an standard defined here: https://datatracker.ietf.org/doc/html/rfc1071
    // Don't worry, isn't that bad, trust me :D, there are just sums (check"SUM")
    uint16_t checksum;
    // identifier for the packet
    uint16_t identifier;
    // the sequence number of the packet
    uint16_t sequence_number;

    /* ICMP data */
    uint32_t payload; // payload data

} icmp_ping_packet_t;
