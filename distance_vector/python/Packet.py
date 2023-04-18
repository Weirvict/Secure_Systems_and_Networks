"""Representation of a packet on the network"""


class Packet:
    """A representation of a packet, they can be constructed and
    queried, but there are not setters."""

    def __init__(self, sim, src, dst, mincost=None):
        self.source = src
        self.dest = dst
        self.sim = sim
        if mincost is None:
            self.mincost = [0] * sim.nentities()
        else:
            self.mincost = []
            for i in mincost:
                self.mincost.append(i)

    @classmethod
    def from_packet(cls, sim, pkt):
        """Construct a packet using a packet as a template"""
        return cls(sim, pkt.get_source(), pkt.get_dest(), pkt.mincost)

    def get_source(self):
        """return the source of this packet"""
        return self.source

    def get_dest(self):
        """return the destination of this packet"""
        return self.dest

    def get_min_cost(self, entity):
        """return the cost of `entity` from this packet"""
        return self.mincost[entity]

    def __repr__(self):
        costs = " ".join([str(i) for i in self.mincost])
        return f"Packet(src={self.source},dst={self.dest},costs=[{costs}])"

    def __str__(self):
        return repr(self)
