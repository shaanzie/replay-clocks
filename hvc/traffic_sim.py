import os
import sys
import zlib
from collections import defaultdict
from copy import deepcopy
from datetime import datetime
from heapq import heappop, heappush
from itertools import combinations_with_replacement
from logging import INFO, Formatter, StreamHandler, getLogger
from logging.handlers import RotatingFileHandler
from random import choice, random, randrange, uniform
# from typing import Callable, MutableMapping
from zipfile import ZIP_DEFLATED, ZipFile

from hvc import HVC

msg_count = 0
utcfromtimestamp = datetime.utcfromtimestamp

def format_time(t):
    return utcfromtimestamp(t).strftime('%H:%M:%S %f')


class TrafficSimulation:

    def __init__(
        self,
        n: int,
        epsilon: int,
        interval: int,
        send_dist,
        recv_dist,
        xmit_dist,
        unit: int,
        send_threshold: float,
        use_leader: bool = False,
        use_hub: bool = False
    ) -> None:

        self.num_nodes = n
        self.unit = unit
        self.use_leader = use_leader
        self.use_hub = use_hub
        assert not (use_hub and use_leader)
        self.nodes = [
            TrafficNode(
                self,
                x,
                n,
                delta_send=send_dist(),
                delta_recv=recv_dist(),
                send_threshold=send_threshold,
                unit=unit,
                interval=interval,
                clock=HVC([epsilon]*n, epsilon, interval, x, n),
                use_leader=use_leader,
                use_hub=use_hub
            ) for x in range(n)
        ]
        self.epsilon = epsilon
        self.xmit_times = defaultdict(dict)  # type: MutableMapping[int, dict[int, int]]
        for x, y in combinations_with_replacement(range(n), 2):
            self.xmit_times[x][y] = self.xmit_times[y][x] = xmit_dist()
        self.lpt_dict = defaultdict(int)

    def send(self, sender, dest):
        global msg_count
        msg_count += 1
        dest_time = self.nodes[dest].phy_clock + self.unit * (self.nodes[sender].delta_send + self.xmit_times[sender][dest] + self.nodes[dest].delta_recv)
        # print('Gonna receive message from {} to {} at time {}'.format(sender, dest, dest_time))
        heappush(
            self.nodes[dest].incoming,
            (dest_time,
            deepcopy(self.nodes[sender].clock), sender)
        )




class TrafficNode:
    def __init__(
        self,
        network: TrafficSimulation,
        n: int,
        num_nodes: int,
        delta_send: int,
        delta_recv: int,
        clock: HVC,
        unit: int,
        interval: int,
        send_threshold: float,
        use_leader: bool = False,
        use_hub: bool = False,
        phy_clock: int = 0,
    ):
        self.network = network
        self.n = n
        self.delta_send = delta_send
        self.delta_recv = delta_recv
        self.clock = clock
        self.unit = unit
        self.interval = interval
        self.send_threshold = send_threshold
        self.phy_clock = phy_clock
        self.use_leader = use_leader
        self.use_hub = use_hub
        self.num_nodes = num_nodes
        self.incoming = []  # type: list[tuple[int, ClockWithBits, int]]
        # heap of time to execute, time sent, sender
        self.logger = getLogger("Node {}".format(self.n))

    def print_log_adv(self):
        # self.logger.info("Advancing: {}, Time: {}, e.epoch: {}, e.offsets: {}, e.counters: {}, f.epoch: {}, f.offsets: {}, f.counters: {}, offset_size: {}, counter_size: {}, epsilon: {}, n: {}, msg_count: {}".format(
        #             self.n,
        #             format_time(self.phy_clock / 2**32),
        #             old_clock.max_epoch,
        #             '-'.join(str(x) for x in old_clock.offsets),
        #             '-'.join(str(x) for x in old_clock.counters),
        #             self.clock.max_epoch,
        #             '-'.join(str(x) for x in self.clock.offsets),
        #             '-'.join(str(x) for x in self.clock.counters),
        #             self.clock.get_offset_size(),
        #             self.clock.get_counter_size(),
        #             self.clock.epsilon,
        #             self.num_nodes,
        #             msg_count
        #         )
        #     )
        # input()
        return 

    def print_log_send(self, sender, clock, old_clock):
        # self.logger.info("From: {}, To: {}, Time: {}, m.epoch: {}, m.offsets: {}, m.counters: {}, e.epoch: {}, e.offsets: {}, e.counters: {}, f.epoch: {}, f.offsets: {}, f.counters: {}, offset_size: {}, counter_size: {}, epsilon: {}, n: {}, msg_count: {}".format(
        #         self.n,
        #         sender,
        #         format_time(time / 2**32),
        #         clock.max_epoch,
        #         '-'.join(str(x) for x in clock.offsets),
        #         '-'.join(str(x) for x in clock.counters),
        #         old_clock.max_epoch,
        #         '-'.join(str(x) for x in old_clock.offsets),
        #         '-'.join(str(x) for x in old_clock.counters),
        #         self.clock.max_epoch,
        #         '-'.join(str(x) for x in self.clock.offsets),
        #         '-'.join(str(x) for x in self.clock.counters),
        #         self.clock.get_offset_size(),
        #         self.clock.get_counter_size(),
        #         self.clock.epsilon,
        #         self.num_nodes,
        #         msg_count
        #     )
        # )
        perceived_epsilon = max(clock.offsets)
        self.logger.info("{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}".format(
                self.n,
                sender,
                self.phy_clock,
                clock.max_epoch,
                '-'.join(str(x) for x in clock.offsets),
                '-'.join(str(x) for x in clock.counters),
                old_clock.max_epoch,
                '-'.join(str(x) for x in old_clock.offsets),
                '-'.join(str(x) for x in old_clock.counters),
                self.clock.max_epoch,
                '-'.join(str(x) for x in self.clock.offsets),
                '-'.join(str(x) for x in self.clock.counters),
                self.clock.get_offset_size(),
                self.clock.get_counter_size(),
                self.clock.epsilon,
                self.num_nodes,
                msg_count,
                self.send_threshold,
                self.interval,
                self.delta_send,
                perceived_epsilon
            )
        )
        # input()
        return 

    def tick(self, threshhold: float = 1):
        
        if random() > threshhold:
            return
        
        global msg_count
        self.phy_clock += MICROSECOND
        old_clock = deepcopy(self.clock)
        self.clock.advance(self.phy_clock)
       
        while self.incoming and self.incoming[0][0] == self.phy_clock:
            time, clock, sender = heappop(self.incoming)
            old_clock = deepcopy(self.clock)
            self.clock.merge(clock, self.phy_clock)

            self.print_log_send(sender, clock, old_clock)
            
            if self.use_hub and self.n == 0 and random() < 0.5:
                self.network.send(self.n, sender)

        if random() < self.send_threshold:
            if self.use_hub and self.n != 0:
                dest = 0
            else:
                dest = randrange(self.network.num_nodes - 1)
                if dest >= self.n:
                    dest += 1
            self.network.send(self.n, dest)

    def recv(self):
        return heappop(self.incoming)


MICROSECOND = 1


def main(
    NUM_NODES=2,
    UNIT=MICROSECOND,
    BITS=3,
    SEND_MIN=1,
    SEND_MAX=12,
    RECV_MIN=1,
    RECV_MAX=13,
    XMIT_MIN=1 * 1000,  # 1 ms - minimum latency (if this is higher than epsilon, offsets and counters must be 0)
    XMIT_MAX=20 * 1000,  # 20 ms - maximum latency
    EPSILON=1 * 1000,  # 1 ms
    INTERVAL=10,
    TIME=1000,  # seconds
    send_threshold=0.008,
    USE_LEADER=False,
    USE_HUB=False
):
    sim = TrafficSimulation(
        NUM_NODES,
        EPSILON,
        INTERVAL,
        lambda: round(uniform(XMIT_MIN, XMIT_MAX)),
        lambda: round(uniform(XMIT_MIN, XMIT_MAX)),
        lambda: round(uniform(XMIT_MIN, XMIT_MAX)),
        UNIT,
        send_threshold,
        USE_LEADER,
        USE_HUB
    )
    

    TIME *= 1000000
    TIME *= MICROSECOND

    log = getLogger('')
    log.setLevel(INFO)

    formatter = Formatter('%(message)s')

    file_name = 'LOGFILES/E{}.N{}.I{}.S{}'.format(EPSILON, NUM_NODES, INTERVAL, int(send_threshold * 1000))
    if USE_HUB:
        file_name = '.'.join((file_name, 'H'))
    if USE_LEADER:
        file_name = '.'.join((file_name, 'L'))

    def rotator(source, dest):
            with open(source, "rb") as sf:
                data = sf.read()
                with ZipFile(file_name + '.log.zip', 'a', compression=ZIP_DEFLATED, compresslevel=9, allowZip64=True) as z:
                    index = min((int(name.split('.')[-1]) for name in z.namelist()), default=0) + 1
                    z.writestr(file_name + '.' + str(index), data)
                compressed = zlib.compress(data, 9)
                with open(dest, "wb") as df:
                    df.write(compressed)
            try:
                os.remove(source)
            except Exception:
                pass

    def namer(name):
        return name + '.gz'

    fh = RotatingFileHandler(file_name + '.log', mode='w', encoding='utf-8', maxBytes=1<<30, backupCount=100000)
    fh.rotator = rotator
    fh.namer = namer
    fh.setLevel(INFO)
    fh.setFormatter(formatter)
    log.addHandler(fh)

    ch = StreamHandler()
    ch.setLevel(INFO)
    ch.setFormatter(formatter)
    log.addHandler(ch)


    # Assert somewhere that clocks don't differ by EPSILON
    while not any(n.phy_clock > TIME for n in sim.nodes):
        
        min_time = min(n.phy_clock for n in sim.nodes)

        if USE_LEADER and (min_time + EPSILON * UNIT > sim.nodes[0].phy_clock):
            sim.nodes[0].tick()
        node = choice(sim.nodes)
        if min_time + EPSILON * UNIT > node.phy_clock:
            node.tick()

        min_time = min(n.phy_clock for n in sim.nodes)
        max_time = max(n.phy_clock for n in sim.nodes)

        assert max_time - min_time <= EPSILON

    fh.close()
    fh.doRollover()
    log.removeHandler(fh)
    return file_name, sim.lpt_dict, BITS


if __name__ == '__main__':

    NUM_NODES = int(sys.argv[1])
    UNIT = int(sys.argv[2])
    EPSILON = int(sys.argv[3])
    INTERVAL = int(sys.argv[4])
    SEND_THRESHOLD = float(sys.argv[5])
    XMIT_MIN = int(sys.argv[6])
    XMIT_MAX = int(sys.argv[7])


    main(
        NUM_NODES=NUM_NODES,
        UNIT=UNIT,
        BITS=3,
        SEND_MIN=XMIT_MIN,
        SEND_MAX=XMIT_MIN,
        RECV_MIN=XMIT_MIN,
        RECV_MAX=XMIT_MIN,
        XMIT_MIN=XMIT_MIN,  # 1 ms
        XMIT_MAX=XMIT_MAX,  # 20 ms
        EPSILON=EPSILON,  # 1 ms
        INTERVAL=INTERVAL,
        TIME=1000,  # seconds
        send_threshold=SEND_THRESHOLD, # On average, this is messages per clock tick per node
        USE_LEADER=True,
        USE_HUB=False
    )