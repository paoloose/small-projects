# Ping implementations

ICMP echo request (ping) implementation in C.

Code is commented for educational purposes, take a look at the code to
understand how it works ✨ (basic understanding of networking is required).

## About ICMP

**ICMP**: Internet Communication Message Protocol :D

ICMP is a protocol that allows to send messages between hosts (many types
of messages are defined!).

ICMP is typically used by network devices (like routers) to send error messages
and operational information about network conditions. It is embedded in the
IP Layer, it works in conjunction with IP to provide a full-functioned
communication system.

- ICMP messages are sent to the IP layer.
- ICMP has many types of messages, but the most common are:
  - **Echo request**: used to ping a host (_the focus of this repo!_)
  - **Echo reply**: response to an echo request (_also important!_)
  - **Destination unreachable**: sent when a host is unreachable.
  - **Time exceeded**: sent when a host is unreachable due to a timeout.
  - Etc.

## Usage

Tested on Ubuntu 22.04 LTS

```sh
gcc -o ping ping.c

sudo ./ping 8.8.8.8
```

## Please note

- Raw sockets are used in this implementations, this feature **requires root privileges**.
- There are other types of "pings", like TCP ping, UDP ping, etc. This repo
  focuses only on ICMP echo requests.
- ICMP packets can be blocked by firewalls, take this into account when
  testing it in your LAN.
    > Why? ICMP is sometimes used by attackers to gather information
    > about a host, or to perform attacks such as a DoS, so firewalls
    > usually block ICMP pings, but this is not something you should worry
    > about.
    >
    > See: [DoS ICMP techniques](https://en.wikipedia.org/wiki/Denial-of-service_attack#Internet_Control_Message_Protocol_(ICMP)_flood).

## Disclaimer

- Implementations are not intended to be CLI tools, but rather examples of
  how to implement ICMP protocol for pinging hosts.
- IPv4 support only.
- No DNS support.

## References

- <https://github.com/davidgatti/How-to-Deconstruct-Ping-with-C-and-NodeJS>
- <https://github.com/execrate0/ft_ping>
- <https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol>
- <https://www.youtube.com/watch?v=AtVWnyDDaDI>
- <https://www.youtube.com/watch?v=e4f2sJ1KF7Q>
- <https://www.coursera.org/learn/tcpip/>

---
Do you see any mistake, or do you have any suggestion?
Contributions are welcome ✨!
