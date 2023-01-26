from socket import *

# This is not a well formed email message (see RFC2822)
msg = "\r\n I love computer networks!"

# This -is- the correct way to end a message after the data command
endmsg = "\r\n.\r\n"

# Choose a mail server (e.g. Google mail server) and call it mailserver
mailserver = "fill me in"
mailport = 4242 # "fill me in"

# Create socket called clientSocket and establish a TCP connection with mailserver
#Fill in start
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
# Fill in start
# Fill in end

# Send RCPT TO command and print server response.
# Fill in start
# Fill in end

# Send DATA command and print server response.
# Fill in start
# Fill in end

# Send message data.
# Fill in start
# Fill in end

# Message ends with a single period.
# Fill in start
# Fill in end

# Send QUIT command and get server response.
# Fill in start
# Fill in end
