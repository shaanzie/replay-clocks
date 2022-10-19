import random
from time import time
from heapq import heappush

from hvc import HVC

# End imports

# Basic node class for
class TrafficNode:

    # Initializes all params
    def __init__(
        self,
        num_nodes: int,             # Number of nodes in the simulation
        epsilon: int,               # Maximum clock skew
        epoch_time: int,            # Epoch interval discretizer
        node_id: int,               # ID of the node
        init_times: list = None,    # Initial times of all nodes (if available)
    ) -> None:
        
        self.num_nodes = num_nodes
        self.epsilon = epsilon
        self.epoch_time = epoch_time
        self.node_id = node_id
        self.init_times = init_times
        self.phy_clock = 0

        self.clock = HVC(
            times = self.init_times,
            epsilon = self.epsilon,
            epoch_interval = self.epoch_time,
            pid = self.node_id,
            max_procs = self.num_nodes
        )                           # Initializing HVC for node

        self.incoming = []          # Heap to store receive interactions

    def get_physical_time(self):
        return self.phy_clock

    # Brings forward the physical clock time of the process
    def sync_elapsed_time(self, elapsed: int):
        self.phy_clock += elapsed

    # Ticks the clock forward by timestep
    def tick(self, timestep: int = 1) -> None:
        self.phy_clock += timestep
        self.clock.advance(phy_clock = self.phy_clock)

    # Simulates a message send and receive to a node
    def send(self, to_node: 'TrafficNode', delta: int) -> None:

        heappush(
            to_node.incoming,
            (delta, self)
        )