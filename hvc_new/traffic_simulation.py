from heapq import heappop
import logging
from logging.handlers import RotatingFileHandler
import math
import random

from traffic_node import TrafficNode

# End imports

'''
Defining logger
'''
# create logger
logger = logging.getLogger("HVC")
logger.setLevel(level=logging.DEBUG)

# set formatter
logFileFormatter = logging.Formatter(
    fmt=f"%(message)s"
)

'''
Logger definitions done
'''

TIMESTEP = 1                                # 1 unit of time = 1 microsecond

class TrafficSimulation:

    def __init__(
        self,
        num_nodes: int,
        epsilon: int,
        epoch_time: int,
        delta: int
    ) -> None:

        self.num_nodes = num_nodes          # Number of nodes in the simulation
        self.epsilon = epsilon              # Allowed clock skew
        self.epoch_time = epoch_time        # Epoch interval

        self.nodes = [TrafficNode(
            num_nodes = self.num_nodes,
            epsilon = self.epsilon,
            epoch_time = self.epoch_time,
            node_id = x,
            init_times = [0]*self.num_nodes
        ) for x in range(self.num_nodes)]   # Initialized nodes

        self.messages = 0
        self.network_clock = 0
        self.delta = delta

    # Tick all nodes forward
    def tick_all(self, tick_size: int = 1):
        for node in self.nodes:
            node.tick(tick_size)

    def simulation(self, max_time: int = math.inf, max_messages: int = math.inf, send_probability: float = 0) -> None:

        while True:
            
            '''
            Things that can happen in each timestep:
            1. Any arbitrary number of nodes can send a message
            2. Any arbitrary number of nodes can receive a message
            3. Any arbitrary number of nodes can do nothing or execute a process
            '''

            for node in self.nodes:

                if random.random() > send_probability:

                    random_node = random.choice(self.nodes)
                    node.send(random_node, random.randint(self.network_clock + 1, self.network_clock + self.delta))
                    self.messages += 1
                
                while node.incoming:

                    phy_time, recv_node = heappop(node.incoming)
                    node.clock.merge(recv_node.clock, self.network_clock)

                    print(   '{network_time},{msg_count},{epsilon},{from_node},{to_node},{from_clock_max_epoch},{from_clock_offsets},{from_clock_counters},{to_clock_max_epoch},{to_clock_offsets},{to_clock_counters},{offset_size},{counter_size}'.format(
                                        network_time = self.network_clock,
                                        msg_count = self.messages,
                                        epsilon = self.epsilon,
                                        from_node = recv_node.node_id,
                                        to_node = node.node_id,
                                        from_clock_max_epoch = recv_node.clock.max_epoch,
                                        from_clock_offsets = '-'.join(str(x) for x in recv_node.clock.offsets),
                                        from_clock_counters = '-'.join(str(x) for x in recv_node.clock.counters),
                                        to_clock_max_epoch = node.clock.max_epoch,
                                        to_clock_offsets = '-'.join(str(x) for x in node.clock.offsets),
                                        to_clock_counters = '-'.join(str(x) for x in node.clock.counters),
                                        offset_size = node.clock.get_offset_size(),
                                        counter_size = node.clock.get_counter_size()
                                    )
                                )


            self.network_clock += TIMESTEP
            self.tick_all(TIMESTEP)

            if self.network_clock == max_time:
                break
                
            if self.messages == max_messages:
                break

if __name__ == '__main__':

    num_nodes = 8
    epsilon = 1 * 100
    epoch_time = 1 * 10
    delta = 5
    send_probability = 0.7

    # set the handler
    fileHandler = RotatingFileHandler(
        filename='hvc_logdir/N{num_nodes}.E{epsilon}.P{epoch_time}.D{delta}.S{send_probability}.log'.format(
            num_nodes = num_nodes,
            epsilon = epsilon,
            epoch_time = epoch_time,
            delta = delta,
            send_probability = send_probability
        ), 
        maxBytes=100_000, 
        backupCount=10
    )
    fileHandler.setFormatter(logFileFormatter)
    fileHandler.setLevel(level=logging.INFO)
    logger.addHandler(fileHandler)

    traffic_sim = TrafficSimulation(
        num_nodes = num_nodes,
        epsilon = epsilon,
        epoch_time = epoch_time,
        delta = delta
    )

    traffic_sim.simulation(send_probability = send_probability)