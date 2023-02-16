# CS3337_Projects
These are the projects from CS3337 Spring 2023
***
## SMTP Client
The SMTP protocol, defined in RFC5321, forms the core of email services for the Internet. Created an SMTP client that interacts with an SMTP server which sends a correctly formed (RFC2822-compliant) message to a server. The server for this class is a special server that accepts pretty much anything handed to it as long as the SMTP protocol is being followed correctly. 
***
## UDP Pinger
UDP sockets to create a ping-like utility and will require the use of proper timeouts and computation of several statistics. Program should send 10 "pings" to the server (simplesmtp.thought.net, UDP port 8192). A response can be considered lost if no reply is received in 1 second. Program should print the response received, compute/display the maximum, minimum, and average round trip time for received packets and the packet loss percentage. The server responds to each message sent to it by just echoing what it receives. It has a maximum packet size of 100 bytes.
***
## Author
* Victoria Weir - victoriaweir@isu.edu
***
### License
This project is licensed under the [MIT License](https://choosealicense.com/licenses/mit/) - see the LICENSE.md file for details.
