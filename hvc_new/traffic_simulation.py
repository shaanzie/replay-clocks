import sys
from heapq import heappush
from random import choice
from copy import deepcopy

from traffic_node import TrafficNode


class TrafficSimulation:

    def __init__(
        self,
        num_nodes: int,
        epsilon: int,
        interval: int,
        delta: int,
        alpha: int,
        unit: int
    ) -> None:
        
        self.num_nodes = num_nodes
        self.nodes = [
            TrafficNode(
                filename = 'LOGDIR/N{}.E{}.D{}.A{}.log'.format(num_nodes, epsilon, alpha, delta),
                node_id=pid,
                num_nodes=num_nodes,
                epsilon=epsilon,
                epoch_interval=interval,
                name='TrafficNode{}'.format(pid),
                unit=unit
            )
            for pid in range(num_nodes)
        ]

        self.epsilon = epsilon*unit
        self.epoch_interval = interval*unit
        self.delta = delta*unit
        self.alpha = alpha
        self.unit = unit

        self.msg_count = 0

    def run_integrity_check(self):

        for node1 in self.nodes:
            for node2 in self.nodes:
                if abs(node1.hvc_clock.max_epoch - node2.hvc_clock.max_epoch) > self.epsilon:
                    raise AssertionError('Clocks differ more than epsilon for clock \n{} and clock \n{}'.format(node1.hvc_clock, node2.hvc_clock))

    def send(self, from_node: TrafficNode, to_node: TrafficNode):

        dest_time = from_node.phy_clock + self.delta
        heappush(
            to_node.receive_queue,
            (dest_time, deepcopy(from_node.hvc_clock))
        )

    def random_simulation(self):

        self.msg_count = 0

        while True:

            for message in range(self.alpha):

                random_node_from = choice(self.nodes)
                random_node_to = choice(self.nodes)
                while random_node_from == random_node_to:
                    random_node_from = choice(self.nodes)
                    random_node_to = choice(self.nodes)

                self.send(random_node_from, random_node_to)
                self.msg_count += 1
            
            for node in self.nodes:
                node.tick(self.alpha, self.delta)

            self.run_integrity_check()
    
    def time_leader_simulation(self):

        leader = self.nodes[0]

        while True:
            
            min_time = min(node.phy_clock for node in self.nodes)
            while min_time + self.epsilon > leader.phy_clock:
                leader.tick(self.alpha, self.delta)

            for message in range(self.alpha):

                random_node_from = choice(self.nodes)
                random_node_to = choice(self.nodes)
                while random_node_from == random_node_to:
                    random_node_from = choice(self.nodes)
                    random_node_to = choice(self.nodes)

                self.send(random_node_from, random_node_to)
                self.msg_count += 1
            
            for node in self.nodes:
                node.tick(self.alpha, self.delta)

            self.run_integrity_check()


if __name__ == '__main__':

    # NUM_NODES = int(sys.argv[1])
    # EPSILON = int(sys.argv[2])
    # INTERVAL = int(sys.argv[3])
    # DELTA = int(sys.argv[4])
    # ALPHA = int(sys.argv[5])
    # UNIT = int(sys.argv[6])

    # DEBUG
    NUM_NODES=4
    EPSILON=10
    INTERVAL=1
    ALPHA=5
    DELTA=1
    UNIT=1000
    

    sim = TrafficSimulation(
        num_nodes=NUM_NODES,
        epsilon=EPSILON,
        interval=INTERVAL,
        delta=DELTA,
        alpha=ALPHA,
        unit=UNIT
    )

    sim.random_simulation()