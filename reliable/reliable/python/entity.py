"""This defines the network entities, Entity is the abstact description of
an Entity where EntityA and EntityB are concrete classes that must be modified
for this assignment"""
import hashlib
import struct
from abc import ABC, abstractmethod
import inspect
import packet

class Entity(ABC):
    """Abstract concept of an Entity"""

    def __init__(self, sim):
        self.sim = sim

    @abstractmethod
    def output(self, message):
        """called from layer5 when a message is ready to be sent by the application"""

    @abstractmethod
    def input(self, packet):
        """called from layer 3, when a packet arrives for layer 4"""

    @abstractmethod
    def timerinterrupt(self):
        """called when timer goes off"""

    @abstractmethod
    def starttimer(self, increment):
        """Provided: call this function to start your timer"""

    @abstractmethod
    def stoptimer(self):
        """Provided: call this function to stop your timer"""

    def tolayer5(self, data):
        """Provided: call this function when you have data ready for layer5"""

    def tolayer3(self, packet):
        """Provided: call this function to send a layer3 packet"""

    # def isACK(self, msg):
    #     return msg == "ACK"
    #
    # def isNAK(self, msg):
    #     return msg == "NAK"
    #
    # def corrupt(self, pkg):
    #     # Getting more detailed information
    #     lengthPkg = pkg[0:10]
    #     seq_numPkg = pkg[10: 20]
    #     checkSumPkg = pkg[20:52]
    #     msg = pkg[52:]
    #
    #     checkSum = hashlib.md5(str(lengthPkg + seq_numPkg + msg).encode('utf-8'))
    #     computedCheckSum = checkSum.hexdigest()
    #     return checkSumPkg != computedCheckSum
    # def checkSum(self, packet):
    #     localSum = 0
    #     variable = 0
    #     packet.__sizeof__()



class EntityA(Entity):
    """Concrete implementaion of EntityA. This entity will receive messages
    from layer5 and must ensure they make it to layer3 reliably"""
    def __init__(self, sim):
        super().__init__(sim)
        print(f"{self.__class__.__name__}.{inspect.currentframe().f_code.co_name} called.")
        self.RTT = 25
        self.CHUNKSIZE = 20
        self.BUFFER = 1000
        self.payload = [20]
        self.seqnum = 0
        self.acknum = 0
        self.checkSum = 0
        self.lastnum = 0
        self.getWin = 8


        # Initialize anything you need here

    def get_check_sum(self, pkt):
        checksum = 0
        checksum += self.seqnum
        checksum += self.acknum
        for i in range(50):
            checksum += self.payload
        return checksum

    def checking(self, msg):
        if msg == 0:
            return "ACK"
        else:
            return "NAK"

    def output(self, message):
        """Called when layer5 wants to introduce new data into the stream"""
        print(f"{self.__class__.__name__}.{inspect.currentframe().f_code.co_name} called.")
        # TODO add some code
        pkt = packet.Packet()
        pkt.payload = message

        if self.checkSum == 0:
            self.tolayer3(pkt)
            self.seqnum += 1
            self.starttimer(1)
            self.checkSum += 1

        elif self.checkSum < self.getWin:
            self.tolayer3(pkt)
            self.seqnum = self.seqnum + 1
            self.checkSum += 1

        print("this is the output stage of entity A")

    def input(self, packet):
        """Called when the network has a packet for this entity"""
        print(f"{self.__class__.__name__}.{inspect.currentframe().f_code.co_name} called.")
        # TODO add some code
        self.acknum = 1
        check = self.checkSum(packet)
        checking = self.checking(check)

        if checking == "ACK":
            self.lastnum += 1
            print("Packet acknum: " + str(self.acknum))
        if checking == "NAK":
            self.lastnum += self.getWin
            self.stoptimer()
            self.checkSum = 0

        print("this is the input stage of entity A")

    def timerinterrupt(self):
        """called when your timer has expired"""
        print(f"{self.__class__.__name__}.{inspect.currentframe().f_code.co_name} called.")

        for i in range(self.seqnum+self.getWin):
            self.lastnum = i

            self.tolayer3(self.lastnum)
        self.starttimer(self.RTT)

        print("timer has been interrupted")

    # From here down are functions you may call that interact with the simulator.
    # You should not need to modify these functions.

    def starttimer(self, increment):
        """Provided: call this function to start your timer"""
        self.sim.starttimer(self, increment)

    def stoptimer(self):
        """Provided: call this function to stop your timer"""
        self.sim.stoptimer(self)

    def tolayer5(self, data):
        """Provided: call this function when you have data ready for layer5"""
        self.sim.tolayer5(self, data)

    def tolayer3(self, packet):
        """Provided: call this function to send a layer3 packet"""
        self.sim.tolayer3(self, packet)

    def __str__(self):
        return "EntityA"

    def __repr__(self):
        return self.__str__()


class EntityB(Entity):
    def __init__(self, sim):
        super().__init__(sim)

        # Initialize anything you need here
        self.RTT = 25
        self.CHUNKSIZE = 20
        self.BUFFER = 1000
        self.payload = [20]
        self.seqnum = 0
        self.acknum = 0
        self.checkSum = 0
        self.lastnum = 0
        self.getWin = 8
        print(f"{self.__class__.__name__}.{inspect.currentframe().f_code.co_name} called.")

    def get_check_sum(self, pkt):
        checksum = 0
        checksum += self.seqnum
        checksum += self.acknum
        for i in range(50):
            checksum += self.payload
        return checksum

    def checking(self, msg):
        if msg == 0:
            return "ACK"
        else:
            return "NAK"


    # Called when layer5 wants to introduce new data into the stream
    # For EntityB, this function does not need to be filled in unless
    # you're doing the extra credit, bidirectional part of the assignment
    def output(self, message):
        print(f"{self.__class__.__name__}.{inspect.currentframe().f_code.co_name} called.")
        # TODO add some code


        print("This is the output stage of Entity B")
        pass

    # Called when the network has a packet for this entity
    def input(self, packet):
        print(f"{self.__class__.__name__}.{inspect.currentframe().f_code.co_name} called.")
        # TODO add some code

        if self.get_check_sum(packet) == 1 and self.checking(packet) == "ACK":
            self.tolayer5(packet.payload)
            self.acknum = self.get_check_sum(packet)
            self.checkSum = packet.payload
            self.tolayer3(packet)
        else:
            self.acknum = self.get_check_sum(packet) - 1
            self.checkSum = packet.payload
            self.tolayer3(packet.payload)

        print("This is the input stage of Entity B")

    # called when your timer has expired
    def timerinterrupt(self):
        print(f"{self.__class__.__name__}.{inspect.currentframe().f_code.co_name} called.")

        print("The timer interrupted entity B")
        pass

    # From here down are functions you may call that interact with the simulator.
    # You should not need to modify these functions.

    def starttimer(self, increment):
        """Provided: call this function to start your timer"""
        self.sim.starttimer(self, increment)

    def stoptimer(self):
        """Provided: call this function to stop your timer"""
        self.sim.stoptimer(self)

    def tolayer5(self, data):
        """Provided: call this function when you have data ready for layer5"""
        self.sim.tolayer5(self, data)

    def tolayer3(self, packet):
        """Provided: call this function to send a layer3 packet"""
        self.sim.tolayer3(self, packet)

    def __str__(self):
        return "EntityB"

    def __repr__(self):
        return self.__str__()
