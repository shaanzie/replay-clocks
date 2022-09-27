from distutils.log import ERROR
from hvc import HVC
from collections import defaultdict
from copy import copy
from datetime import datetime
from json import dumps
from hashlib import md5
from heapq import heappush, heappop
from itertools import combinations_with_replacement
from logging import getLogger, Formatter, StreamHandler, DEBUG, INFO, ERROR
from logging.handlers import RotatingFileHandler
from math import ceil, log2
from pathlib import Path
from re import compile
from random import choice, random, randrange, uniform
# from typing import Callable, MutableMapping
from zipfile import ZipFile, ZIP_DEFLATED
from zlib import decompress


import os
import sys
import zlib

utcfromtimestamp = datetime.utcfromtimestamp


class TrafficSimulation:

    def __init__(
        self,
        n: int,
        epsilon: int,
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
                delta_send=send_dist(),
                delta_recv=recv_dist(),
                send_threshold=send_threshold,
                unit=unit,
                clock=HVC([0]*n, epsilon, x),
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
        heappush(
            self.nodes[dest].incoming,
            (self.nodes[dest].phy_clock + self.unit * (self.nodes[sender].delta_send + self.xmit_times[sender][dest] + self.nodes[dest].delta_recv),
             copy(self.nodes[sender].clock), sender)
        )


def format_time(t):
    return utcfromtimestamp(t).strftime('%H:%M:%S %f')

class TrafficNode:
    def __init__(
        self,
        network: TrafficSimulation,
        n: int,
        delta_send: int,
        delta_recv: int,
        clock: HVC,
        unit: int,
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
        self.send_threshold = send_threshold
        self.phy_clock = phy_clock
        self.use_leader = use_leader
        self.use_hub = use_hub
        self.incoming = []  # type: list[tuple[int, ClockWithBits, int]]
        # heap of time to execute, time sent, sender
        self.logger = getLogger("Node {}".format(self.n))

    def tick(self, threshhold):
        #if random() > threshhold:
            #return
        self.phy_clock += self.unit
        self.clock.advance(self.phy_clock)
        while self.incoming:
            time, clock, sender = heappop(self.incoming)
            self.clock.merge(clock, self.phy_clock)
            self.logger.info("To {} From {} at {}: \nPrev_Clock: {}. \nNow Clock {}".format(
                    self.n,
                    sender,
                    format_time(time / 2**32),
                    clock,
                    self.clock
                )
            )
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
    NUM_NODES=8,
    UNIT=MICROSECOND,
    BITS=3,
    SEND_MIN=1,
    SEND_MAX=12,
    RECV_MIN=1,
    RECV_MAX=13,
    XMIT_MIN=1 * 1000,  # 1 ms
    XMIT_MAX=20 * 1000,  # 20 ms
    EPSILON=400 * 1000,  # 400 ms
    TIME=1000,  # seconds
    send_threshold=0.008,
    USE_LEADER=False,
    USE_HUB=False
):
    sim = TrafficSimulation(
        NUM_NODES,
        EPSILON,
        lambda: round(uniform(SEND_MIN, SEND_MAX)),
        lambda: round(uniform(RECV_MIN, RECV_MAX)),
        lambda: round(uniform(XMIT_MIN, XMIT_MAX)),
        UNIT,
        send_threshold,
        USE_LEADER,
        USE_HUB
    )
    log = getLogger('')
    log.setLevel(ERROR)

    formatter = Formatter('%(message)s')

    file_name = 'LOGFILES/E{}.N{}.B{}.S{}'.format(EPSILON, NUM_NODES, BITS, int(send_threshold * 1000))
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

    TIME *= 1000000
    TIME *= MICROSECOND
    while not any(n.phy_clock > TIME for n in sim.nodes):
        min_time = min(n.phy_clock for n in sim.nodes)
        if USE_LEADER and (min_time + EPSILON * UNIT > sim.nodes[0].phy_clock):
            sim.nodes[0].tick(1)
        node = choice(sim.nodes)
        if min_time + EPSILON * UNIT > node.phy_clock:
            node.tick(1)
    fh.close()
    fh.doRollover()
    log.removeHandler(fh)
    return file_name, sim.lpt_dict, BITS


def time_prefix(start):
    t = time()
    st = t - start
    return '[{} {}/{}]: '.format(int(st // 86400), format_time(st), format_time(t))


if __name__ == '__main__':
    from functools import partial
    from itertools import product
    from multiprocessing import Pool, cpu_count
    from time import time

    NAMES = [
        "Calypso",
        "Euclid",
        "Delhi",
        "Denver",
        "Detroit",
        "GreatLakes",
        "GreaterLansing",
        "MackinacIsland",
        "Mason",
        "Wyoming"
    ]

    WEIGHTS = [
        14 * 15,    # Calypso
        16 * 7,     # Euclid
        7 * 7,      # Delhi
        7 * 2,      # Denver
        7 * 4,      # Detroit
        7 * 4,      # GreatLakes
        7 * 4,      # GreaterLansing
        7 * 4,      # MackinacIsland
        7 * 7,      # Mason
        7 * 4,      # Wyoming
    ]  # time when first job was at 16s * num cores allocated

    dialog = "\n".join((
        ["Which node am I?"] +
        ["{}:\t{}".format(idx, name) for idx, name in enumerate(NAMES)] +
        [""]
    ))
    while True:
        resp = input(dialog)
        if resp == 'N/A':
            ID = None
        else:
            try:
                ID = int(resp)
            except Exception:
                continue
        break
    start = time()
    TOTAL = sum(WEIGHTS)

    working_set = list(product(
        (8, 16, 32, 64),                         # num_nodes
        (400 * 1000 // 2**x for x in range(7)),  # epsilon
        range(3, 9),                             # bits
        # (2**x / 1000 for x in range(3, 7)),      # send rate
        (2**x / 1000 for x in (0, *range(3, 7))),      # send rate
        # (8, 16, 32, 64),                         # num_nodes
        # ((1 * 1000, 20 * 1000), )                # xmit_range
        # [(False, False)],                        # use_hub, use_leader
        [(True, False), (False, True)],          # use_hub, use_leader
    ))

    # to_remove = []
    # for tup in working_set:
    #     _, _, send_rate, num_nodes, (xmit_low, xmit_high) = tup
    #     if send_rate == 0.008 and num_nodes == 8 and xmit_low == 1000 and xmit_high = 20000:
    #         to_remove.append(tup)
    # for r in to_remove:
    #     working_set.remove(r)

    lw = STOP = len(working_set)
    checksum = md5(dumps(working_set).encode()).hexdigest().upper()
    if ID is not None:
        START = sum(WEIGHTS[:ID]) * lw // TOTAL
        STOP = sum(WEIGHTS[:ID + 1]) * lw // TOTAL
        working_set = working_set[START:STOP]
    else:
        START = 0
    response = ''

    num_cores = max(
        min(4, cpu_count()),
        cpu_count() - 1
    )

    def translator(tup):
        # epsilon, bits, send_rate, num_nodes, (xmit_low, xmit_high) = tup
        # epsilon, bits, send_rate, num_nodes, (USE_HUB, USE_LEADER) = tup
        num_nodes, epsilon, bits, send_rate, (USE_HUB, USE_LEADER) = tup
        return main(
            NUM_NODES=num_nodes,
            BITS=bits,
            EPSILON=epsilon,
            send_threshold=send_rate,
            USE_HUB=USE_HUB,
            USE_LEADER=USE_LEADER,
            # XMIT_MIN=xmit_low,
            # XMIT_MAX=xmit_high
        )

    while not response.lower().startswith('y'):
        response = input((
            '{0}I am {1}. Please verify this is correct.  Checksum: {2}.\n'
            '{0}{3} jobs now queued ({4}-{5}). Total size {6}. This machine will use {7} worker cores. (y/n)? '
        ).format(time_prefix(start), "N/A" if ID is None else NAMES[ID], checksum, len(working_set), START, STOP - 1, lw, num_cores))
        if response.lower().startswith('n'):
            exit(1)
    t = time()
    print(time_prefix(start), 'Starting')
    with Pool(num_cores, initializer=partial(os.nice, 20)) as pool:
        for idx, (file_name, lpt_dict, u) in enumerate(pool.imap_unordered(translator, working_set), 1):
            t = time()
            print(time_prefix(start), '{:%}  ({}/{})'.format(idx / len(working_set), idx, len(working_set)))
            print(time_prefix(start), file_name)
            sys.stdout.flush()
            
            # uncomment the following to enable parsing from generated log files:
            # overflow_count = [0]
            # lpt_dict = defaultdict(int)
            # expr = compile(r'.*lpt=(\d+).*lpt=(\d+)')

            # def parse_line(line):
            #     if 'Overflow' in line:
            #         overflow_count[0] += 1
            #         return
            #     match = expr.match(line)
            #     if match:
            #         first, second = match.groups()
            #         lpt_dict[int(second)] += 1

            # for name in sorted(Path('.').glob(file_name + '*.log*')):
            #     print(time_prefix(start), 'Reading', name, '...')
            #     sys.stdout.flush()
            #     breakpoint()
            #     if '.zip' in name.suffixes:
            #         # with ZipFile(name, 'r', compression=ZIP_DEFLATED, compresslevel=9, allowZip64=True) as z:
            #         #     for subfile in z.namelist():
            #         #         print(time_prefix(start), 'Reading', "/".join((name.name, subfile)), '...')
            #         #         with z.open(subfile) as f:
            #         #             for line in f.read().decode().splitlines():
            #         #                 parse_line(line)
            #         with name.open('rb') as f:
            #             for line in decompress(f.read()).decode().splitlines():
            #                 parse_line(line)
            #     else:
            #         with name.open('r') as g:
            #             for line in g.read().splitlines():
            #                 parse_line(line)
            #     os.remove(name)

            bit_values = []
            check = 1 << u
            overflow_count = sum((key >= check) * value for key, value in lpt_dict.items())
            for i in range(ceil(log2(max(1, max(lpt_dict or (0, ))))) + 1):
                total = 0
                check = 1 << i
                for key in lpt_dict:
                    if key < check:
                        total += lpt_dict[key]
                bit_values.append(total)
            print(time_prefix(start), overflow_count, sum(lpt_dict.values()), *bit_values)
            sys.stdout.flush()