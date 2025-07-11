# Secure Systems and Networks Projects
These are the projects from Secure Systems and Networks Spring 2023
***
## SMTP Client
The SMTP protocol, defined in RFC5321, forms the core of email services for the Internet. Created an SMTP client that interacts with an SMTP server which sends a correctly formed (RFC2822-compliant) message to a server. The server for this class is a special server that accepts pretty much anything handed to it as long as the SMTP protocol is being followed correctly. 
***
## UDP Pinger
UDP sockets to create a ping-like utility and will require the use of proper timeouts and computation of several statistics. Program should send 10 "pings" to the server (simplesmtp.thought.net, UDP port 8192). A response can be considered lost if no reply is received in 1 second. Program should print the response received, compute/display the maximum, minimum, and average round trip time for received packets and the packet loss percentage. The server responds to each message sent to it by just echoing what it receives. It has a maximum packet size of 100 bytes.
***
## Reliable Transport Protocol
Project for sending and recieving transport-level code for implementing a simple reliable data transfer protocol.There are two versions of this lab, the Alternating-Bit-Protocol version and the  Go-Back-N version. It will be simulated hardware/software environment. Stopping/starting of timers are also simulated, and timer interrupts will cause the timer handling routine to be activated.
***
## Distance Vector Routing
Tasked to be writing a "distributed" set of procedures that implement a distributed asynchronous distance vector routing for the network. 
## Author
* Victoria Weir - victoriaweir@isu.edu
***
### License
This project is licensed under the [MIT License](https://choosealicense.com/licenses/mit/) - see the LICENSE.md file for details.
