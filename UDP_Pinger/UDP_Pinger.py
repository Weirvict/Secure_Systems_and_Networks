from socket import *
import time

# Sever
server = "simplesmtp.though.net"
UDPport = 8192

# UDP socket creation and UDP connection with sever
clientSocket= socket(AF_INET, SOCK_DGRAM)
clientSocket.connect((server,UDPport))

# The following functions should use the time for received packets and the packet loss percentage to be able to compute and display it
# Function for printing, should return format of print
def print_response(time, lost):
    # Gets the lost packet's percentage and print format
    lostLength = len(lost)
    computeLostPercentage = (lostLength//10) * 100
    lostPercentFormat = "Lost = " + str(lostLength) + " (" + str(computeLostPercentage) + "%)" + " "
    mini = "Minimum: " + str(min(time)) + " "
    maxa = "Maximum: " + str(max(time)) + " "

    # Combined all of the data from the 10 pings
    combined = mini + maxa + compute_average(time) + lostPercentFormat
    return print(combined)

# Function to get average and displays it
def compute_average(time):
    length = len(time)
    x = 0
    amount = 0
    # adds all the times together
    for x in range (length):
        amount += time[x]
        x+=1
    # gets average
    amount = amount//length
    return "Average = " + str(amount) + " "

def main():
    arrayTime = [] # For all the time and can be used to compare eachother
    arrayLost = [] # For the lost packets that went above 1s in elapsed time

    # should iterate 10 times
    for packetNum in range(0,10):
        # Initalize timer
        initialTime = time.time()
        packetNum += 1

        #pings
        message = "Packet " + str(packetNum)
        clientSocket.sendto(str(initialTime).encode('utf-8'), (server,UDPport))
        recv = clientSocket.recv(1024).decode()
        print(recv)
        if recv[:3] != '250':
            print('250 reply not recieved from sever')

        # end ping

        # end timer and calculate elapsed time
        end_time = time.time()
        elapsed_time = (end_time-initialTime)
        #check to see if packet has been lost and if so adds it to arrayLost
        if elapsed_time > 1:
            arrayLost.append(elapsed_time)
        #appends all the time together
        arrayTime.append(elapsed_time)

        #Formats the print line
        print("The time it took for " + message + " is in milli-seconds " + str(elapsed_time))
    # get response
    print_response(arrayTime,arrayLost)

main()
