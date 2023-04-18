"""this is the 1st entity; you need to modify this to
   implement the distance routing algorithm for this entity
"""

from Entity import Entity
from Packet import Packet


class Entity1(Entity):
    """Implementation of Entity1"""

    def __init__(self, sim, startvector):
        """Perform any necessary initialization in the constructor.
        startvector is the initial costs known to this node.
        startvector[1] is 0 (no cost to talk to yourself) and if
        startvector[n] is 999, then this node and 'n' are not connected"""
        super().__init__(sim)
        self.sim = sim

        # create NxN matrix for distance storage
        self.distance_table = []
        for _ in range(len(startvector)):
            self.distance_table.append([999] * len(startvector))

        # your intialization code here

    def start(self):
        """This function is called once the simulator is fully
        ready handle packets. You can start to send packets in
        this function."""

    def update(self, pkt):
        """Handle updates when a packet is received.  Students will need to call
        NetworkSimulator.toLayer2() with new packets based upon what they
        send to update.  Be careful to construct the source and destination of
        the packet correctly.
        details."""

    def link_cost_change_handler(self, which_link, new_cost):
        """This function is called when the topology of the network
        has changed. `which_link` is the number of the neighbor
        for whom the cost has changed and `new_cost` is the new cost
        of that path"""

    def print_dt(self):
        """Called at simulation end (or for debugging) to print
        the distance table for this node."""

        print("         via")
        print(" D1 |   0   2")
        print("----+--------")
        for i in range(self.sim.nentities()):
            if i == 1:
                continue
            print(f"   {i}|", end='')
            for j in range(self.sim.nentities()):
                if j == 1 or j == 3:
                    continue

                print(f"{self.distance_table[i][j]:>5}", end='')
            print('')
