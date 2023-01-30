from socket import *

# This is not a well formed email message (see RFC2822)
msg = "\r\n I love computer networks!"

# This -is- the correct way to end a message after the data command
endmsg = "\r\n.\r\n"

# Choose a mail server (e.g. Google mail server) and call it mailserver
mailserver = "simplesmtp.thought.net"
mailport = 8025

# Create socket called clientSocket and establish a TCP connection with mailserver
#Fill in start
clientSocket= socket(AF_INET,SOCK_STREAM)
clientSocket.connect(mailserver,mailport)
#Fill in end

# NOTE: you should write better handling for return messages. The
# code below might work, but it does NOT handle multiline responses.
# Please consult RFC5321 for details.

recv = clientSocket.recv(1024).decode()
print(recv)
if recv[:3] != '220':
    print('220 reply not received from server.')

# Send HELO command and print server response.
heloCommand = 'HELO Alice\r\n'
clientSocket.send(heloCommand.encode())
recv1 = clientSocket.recv(1024).decode()
print(recv1)
if recv1[:3] != '250':
    print('250 reply not received from server.')

# Send MAIL FROM command and print server response.
# Fill in start and found on page 87 in the RFC5321
mailCommand = 'MAIL FROM: <weirvict@isu.edu>\r\n'
clientSocket.send(mailCommand.encode())
recv2 = clientSocket.recv(1024).decode()
print(recv2)
if recv2[:3] != '250':
    print('250 remply not recieved from sever')
# Fill in end

# Send RCPT TO command and print server response.
# Fill in start
rcptToCommand = 'RCPT TO: <weirvict@isu.edu>\r\n'
clientSocket.send(rcptToCommand.encode(1024))
recv3 = clientSocket.recv(1024).decode()
print(recv3)
if recv3[:3] != '250':
    print('250 reply not received from server.')
# Fill in end

# Send DATA command and print server response.
# Fill in start
dataCommand = 'DATA \n\r'
clientSocket.send(dataCommand.encode())
recv4 = clientSocket.recv(1024).decode()
print(recv4)
if recv4[:3] != '250':
    print('250 reply not received from server.')
# Fill in end

# Send message data.
# Fill in start
messageDataCommand = 'Hi'
clientSocket.send(messageDataCommand.encode())
recv5 = clientSocket.recv(1024).decode()
print(recv5)
if recv5[:3] != '250':
    print('250 reply not received from server.')
# Fill in end

# Message ends with a single period.
# Fill in start
clientSocket.send(endmsg.encode())
recv6 = clientSocket.recv(1024).decode()
print(recv6)
if recv6[:3] != '250':
    print('250 reply not received from server.')
# Fill in end

# Send QUIT command and get server response.
# Fill in start
quitCommand = 'QUIT'
clientSocket.send((quitCommand.encode()))
recv7 = clientSocket.recv(1024).decode()
print(recv7)
if recv7[:3] != '250':
    print('250 reply not received from server.')

# Fill in end
