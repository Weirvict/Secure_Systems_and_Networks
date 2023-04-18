"""Entity - generic definitions common to call entities"""

from abc import ABC, abstractmethod


class Entity(ABC):
    """Generic (abtract) representation of an Entity"""

    def __init__(self, sim):
        self.distance_table = []
        self.sim = sim
        for _ in range(sim.nentities()):
            self.distance_table.append([0] * sim.nentities())

    @abstractmethod
    def update(self, pkt):
        """The update function.  Will have to be written in subclasses by students"""

    @abstractmethod
    def link_cost_change_handler(self, which_link, new_cost):
        """The link cost change handlder.  Will have to be written in appropriate
        subclasses by students.  Note that only Entity0 and Entity1 will need
        this, and only if extra credit is being done"""

    # Print the distance table of the current entity.
    @abstractmethod
    def print_dt(self):
        """print the distance tree as understood by this node"""

    @abstractmethod
    def start(self):
        """Called exactly once when the simulator is ready to start
        handling packets"""

    def print_dt_helper(self, entity):
        """called by subclasses to print the tree from this node"""
        for ent in range(self.sim.nentities()):
            if ent == entity:
                continue

            print(f"   {ent:<4}|", end="")
            for i in range(self.sim.nentities()):
                print(f"{self.distance_table[ent][i]:>4}", end="")
            print("")
