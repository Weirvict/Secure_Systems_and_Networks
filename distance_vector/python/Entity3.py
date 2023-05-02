"""this is the 3rd entity; you need to modify this to
   implement the distance routing algorithm for this entity
"""

from Entity import Entity
from Packet import Packet


class Entity3(Entity):
    """Implementation of Entity3"""

    def __init__(self, sim, startvector):
        """Perform any necessary initialization in the constructor.
        startvector is the initial costs known to this node.
        startvector[3] is 0 (no cost to talk to yourself) and if
        startvector[n] is 999, then this node and 'n' are not connected"""
        super().__init__(sim)
        self.sim = sim

        # create NxN matrix for distance storage
        self.distance_table = []
        for _ in range(len(startvector)):
            self.distance_table.append([999] * len(startvector))

        # your intialization code here
        # Packets being defined for cost
        self.distance_table[0][0] = self.sim.cost[0][3]
        self.distance_table[1][1] = self.sim.cost[1][3]
        self.distance_table[2][2] = self.sim.cost[2][3]
        self.distance_table[3][3] = self.sim.cost[3][3]

        self.pkt_minCost = self.sim.NUMENTITIES

        for i in range(self.sim.NUMENTITIES):
            self.pkt_minCost += self.distance_table[i][i]


    def start(self):
        """This function is called once the simulator is fully
        ready handle packets. You can start to send packets in
        this function."""
        # Starting packets
        packet1 = Packet(self.sim, 3, 0, self.distance_table)
        self.sim.to_layer2(3, packet1)
        packet2 = Packet(self.sim, 3, 2, self.distance_table)
        self.sim.to_layer2(3, packet2)

    def update(self, pkt):
        """Handle updates when a packet is received.  Students will need to call
        NetworkSimulator.toLayer2() with new packets based upon what they
        send to update.  Be careful to construct the source and destination of
        the packet correctly.
        details."""

        # newcost = 0
        # for i in range(self.sim.NUMENTITIES):
        #     booli = False
        #     newcost = self.distance_table[pkt.getSource()][pkt.getSource()] + pkt.getMincost(i)
        #     self.distance_table[i][pkt.getSource()] = newcost
        #
        #     if ((self.distance_table[i][pkt.getSource()]) < self.pkt_minCost[i]):
        #         self.pkt_minCost[i] = self.distance_table[i][pkt.getSource()]
        #         booli = True
        #
        #     if (booli == True):
        #         packet1 = Packet(self.sim, 3, 0, self.distance_table)
        #         self.sim.to_layer2(3, packet1)
        #         packet2 = Packet(self.sim, 3, 2, self.distance_table)
        #         self.sim.to_layer2(3, packet2)

        # Packets being updated
        if pkt.source == 1:
            self.distance_table[2][1] = pkt.mincost[2][2] + self.distance_table[1][1]
        if pkt.source == 2:
            self.distance_table[1][2] = pkt.mincost[1][1] + self.distance_table[2][2]
            self.distance_table[3][2] = pkt.mincost[3][3] + self.distance_table[2][2]
        if pkt.source == 3:
            self.distance_table[2][3] = pkt.mincost[2][2] + self.distance_table[3][3]



    def link_cost_change_handler(self, which_link, new_cost):
        """This function is called when the topology of the network
        has changed. `which_link` is the number of the neighbor
        for whom the cost has changed and `new_cost` is the new cost
        of that path"""

        # Attempted code
        self.distance_table[which_link][which_link] = new_cost
        self.pkt_minCost[which_link] = self.distance_table[which_link][which_link]

        packet1 = Packet(self.sim, 3, 0, self.distance_table)
        self.sim.to_layer2(3, packet1)
        packet2 = Packet(self.sim, 3, 2, self.distance_table)
        self.sim.to_layer2(3, packet2)


    def print_dt(self):
        """Called at simulation end (or for debugging) to print
        the distance table for this node."""
        print("         via")
        print(" D3 |   0   2")
        print("----+--------")
        for i in range(self.sim.nentities()):
            if i == 3:
                continue
            
            print(f"   {i}|", end='')
            for j in range(self.sim.nentities()):
                if j == 1 or j == 3:
                    continue

                print(f"{self.distance_table[i][j]:>5}", end='')
            print('')
