class Event:
    # These constants are possible events
    FROMLAYER2 = 0
    LINKCHANGE = 1

    def __init__(self, time, etype, entity, pkt=None):
        self.setTime(time)
        self.setType(etype)
        self.setEntity(entity)
        self.setPacket(pkt)

    def setTime(self, time):
        assert time >= 0.0, "only positive time allowed"
        self.time = time

    def getTime(self):
        return self.time

    def setType(self, etype):
        assert etype in (
            Event.FROMLAYER2,
            Event.LINKCHANGE,
        ), f"incorrect event type: {etype}"
        self.etype = etype

    def getType(self):
        return self.etype

    def setEntity(self, entity):
        self.entity = entity

    def getEntity(self):
        return self.entity

    def setPacket(self, pkt):
        self.packet = pkt

    def getPacket(self):
        return self.packet

    @staticmethod
    def sorter(e):
        return e.getTime()

    def __repr__(self):
        if self.etype == Event.FROMLAYER2:
            strtype = "FROMLAYER2"
        elif self.etype == Event.LINKCHANGE:
            strtype = "LINKCHANGE"
        else:
            strtype = "unknown!"

        pktstr = "None"
        if pktstr is not None:
            pktstr = repr(self.packet)

        return (
            f"Event(time={self.time},type={strtype},entity={self.entity},pkt={pktstr})"
        )

    def __str__(self):
        return repr(self)
