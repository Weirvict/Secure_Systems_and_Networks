import random

from Entity0 import Entity0
from Entity1 import Entity1
from Entity2 import Entity2
from Entity3 import Entity3
from Event import Event
from Packet import Packet


class NetworkSimulator:
    # This is the number of entities in the simulator
    NUMENTITIES = 4

    def __init__(self, link_changes, trace, seed):
        # Parameters of the simulation
        self.traceLevel = trace
        self.event_list = []

        random.seed(seed)

        self.cost = [[0, 1, 3, 7], [1, 0, 1, 999], [3, 1, 0, 2], [7, 999, 2, 0]]

        self.entity = [None] * NetworkSimulator.NUMENTITIES
        self.entity[0] = Entity0(self, self.cost[0])
        self.entity[1] = Entity1(self, self.cost[1])
        self.entity[2] = Entity2(self, self.cost[2])
        self.entity[3] = Entity3(self, self.cost[3])

        self.time = 0.0

        if link_changes:
            self.event_list.append(Event(10000.0, Event.LINKCHANGE, 0))
            self.event_list.sort(key=Event.sorter)

    def runSimulator(self):
        """run the simulator. Does not return until the event list is empty"""

        # send start signal
        [entity.start() for entity in self.entity]

        # now, process events until done
        while len(self.event_list) > 0:
            e = self.event_list.pop(0)

            if self.traceLevel > 1:
                print("")
                print(f"event received. {repr(e)}")

            self.time = e.getTime()

            if e.getType() == Event.FROMLAYER2:
                assert (
                    e.getEntity() is not None
                    and e.getEntity() >= 0
                    and e.getEntity() < len(self.entity)
                ), f"invalid entity in event: {e.getEntity()}"
                self.entity[e.getEntity()].update(e.getPacket())

            elif e.getType() == Event.LINKCHANGE:
                if self.time < 10001.0:
                    self.cost[0][1] = 20
                    self.cost[1][0] = 20
                    self.entity[0].link_cost_change_handler(1, 20)
                    self.entity[1].link_cost_change_handler(0, 20)
                else:
                    self.cost[0][1] = 1
                    self.cost[1][0] = 1
                    self.entity[0].linkCostChangeHandler(1, 1)
                    self.entity[1].linkCostChangeHandler(0, 1)

            else:
                assert False, f"invalid event type: {e.getType()}"

        print(f"Simulator terminated at t={self.time}, no packets in medium.")

        for e in self.entity:
            e.print_dt()

    def getLastPacketTime(self, evtfrom, evtto):
        time = 0.0
        for evt in self.event_list:
            if evt.getType() != Event.FROMLAYER2:
                continue
            if evt.getEntity() != evtto:
                continue
            if evt.getPacket().get_source() != evtfrom:
                continue
            time = evt.getTime()
        return time

    def to_layer2(self, entity, p):
        assert p.get_source() >= 0 and p.get_source() < len(
            self.entity
        ), f"illegal packet source: {p.get_source()}"
        assert p.get_dest() >= 0 and p.get_dest() < len(
            self.entity
        ), f"illegal packet destination: {p.get_dest()}"
        assert (
            p.get_source() != p.get_dest()
        ), f"source and destination the same: {p.get_source()}"
        assert (
            self.cost[p.get_source()][p.get_dest()] != 999
        ), f"source {p.get_source()} and destination {p.get_dest()} not connected"
        assert (
            p.get_source() == entity
        ), f"entity {entity.entitynumber} can't send packets from {p.get_source()}"

        if self.traceLevel > 2:
            print(f"to_layer2(): {repr(p)}")

        arrival_time = self.getLastPacketTime(p.get_source(), p.get_dest())
        if arrival_time == 0.0:
            arrival_time = self.time
        arrival_time = arrival_time + 1.0 + (random.random() * 9.0)

        if self.traceLevel > 2:
            print("to_layer2(): Scheduling arrival of packet.")
        current_packet = Packet.from_packet(self, p)
        e = Event(
            arrival_time, Event.FROMLAYER2, current_packet.get_dest(), current_packet
        )

        self.event_list.append(e)
        self.event_list.sort(key=Event.sorter)

    def nentities(self):
        """the number of entities in this network"""
        return len(self.entity)
