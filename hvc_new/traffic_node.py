from heapq import heappop
from copy import deepcopy
import time

from logger import Logger
from hvc import HVC

class TrafficNode:

    def __init__(
        self, 
        filename: str,
        node_id: int,
        num_nodes: int,
        epsilon: int,
        epoch_interval: int,
        unit: int,
        name: str = 'TrafficNode'
    ) -> None:
        
        # self.logger = Logger(filename = filename, name = 'TrafficNode{}'.format(node_id)).logger
        self.hvc_clock = HVC(
            times=[epsilon]*num_nodes, 
            epsilon=epsilon,
            epoch_interval=epoch_interval*unit,
            pid=node_id,
            max_procs=num_nodes
        )
        self.phy_clock = 0 
        self.receive_queue = []
        self.unit = unit
    
    # Prints logs
    def print_log(
        self, 
        from_node: int,
        to_node: int,
        sender_hvc: HVC,
        recv_hvc: HVC,
        node_hvc: HVC,
        alpha: int,
        delta: int
    ) -> None:

        print(
            '{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}'.format(
                from_node,
                to_node,
                sender_hvc.max_epoch,
                '-'.join(str(x) for x in sender_hvc.offsets),
                sender_hvc.get_offset_size(),
                '-'.join(str(x) for x in sender_hvc.counters),
                sender_hvc.get_counter_size(),
                recv_hvc.max_epoch,
                '-'.join(str(x) for x in recv_hvc.offsets),
                recv_hvc.get_offset_size(),
                '-'.join(str(x) for x in recv_hvc.counters),
                recv_hvc.get_counter_size(),
                node_hvc.max_epoch,
                '-'.join(str(x) for x in node_hvc.offsets),
                node_hvc.get_offset_size(),
                '-'.join(str(x) for x in node_hvc.counters),
                node_hvc.get_counter_size(),
                max(node_hvc.counters),
                node_hvc.epsilon,
                node_hvc.get_perceived_e_drift(),
                node_hvc.interval,
                alpha,
                delta
            )
        )

    # Advances clock by 1
    def tick(self, alpha, delta) -> None:

        while len(self.receive_queue) > 0 and self.receive_queue[0][0] == self.phy_clock:

            sender_pc, sender_hvc = heappop(self.receive_queue)
            recv_hvc = deepcopy(self.hvc_clock)

            # print('*'*80)
            # print('\nSender: \n', sender_hvc)
            # print('\nReceiver: \n', recv_hvc)

            self.hvc_clock.merge(sender_hvc, self.phy_clock)

            # print('\nUpdated receiver clock: \n', self.hvc_clock)
            # input()

            self.print_log(
                from_node=sender_hvc.pid,
                to_node=recv_hvc.pid,
                sender_hvc=sender_hvc,
                recv_hvc=recv_hvc,
                node_hvc=self.hvc_clock,
                alpha = alpha,
                delta = delta
            )

        self.phy_clock += self.unit
        
        self.hvc_clock.advance(self.phy_clock)

            