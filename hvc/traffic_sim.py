import time

MAX_NUM_PROCESSES=64

class ClockWithBits:
    def __init__(
        self,
        hpt: int,
        lpt: int,
        u: int
    ):
        self.hpt = hpt
        self.lpt = lpt
        self.u = u

    def __repr__(self):
        return "ClockWithBits(hpt={}, lpt={}, u={})".format(self.hpt, self.lpt, self.u)

    def advance(self, physical_input, pt_only=False):
        if physical_input > (self.hpt << self.u):
            self.hpt = physical_input >> self.u
            self.lpt = 0
        elif not pt_only:
            self.lpt += 1
            # if self.lpt >= (1 << self.u):
            #     getLogger().error("Overflow! {}".format(self))

    def as_int(self) -> int:
        return (self.hpt << self.u) + self.lpt

    def merge(self, other: 'ClockWithBits'):
        as_int = other.as_int()
        new_t = as_int >> self.u
        if new_t > self.hpt:
            self.advance(as_int)
        elif new_t == self.hpt:
            self.lpt = max(self.lpt, as_int % (1 << self.u)) + 1
            # if self.lpt >= (1 << self.u):
            #     getLogger().error("Overflow! {}".format(self))

    def __eq__(self, other: 'ClockWithBits') -> bool:
        return (self.hpt, self.lpt) == (other.hpt, other.lpt)

    def __ne__(self, other: 'ClockWithBits') -> bool:
        return (self.hpt, self.lpt) != (other.hpt, other.lpt)

    def __lt__(self, other: 'ClockWithBits') -> bool:
        return (self.hpt, self.lpt) < (other.hpt, other.lpt)

    def __le__(self, other: 'ClockWithBits') -> bool:
        return (self.hpt, self.lpt) <= (other.hpt, other.lpt)

    def __gt__(self, other: 'ClockWithBits') -> bool:
        return (self.hpt, self.lpt) > (other.hpt, other.lpt)

    def __ge__(self, other: 'ClockWithBits') -> bool:
        return (self.hpt, self.lpt) >= (other.hpt, other.lpt)

class HVC:

    def __init__(self, clock: ClockWithBits, pid: int, epsilon: float) -> None:
        
        self.pwc = clock
        self.epsilon = epsilon
        self.epoch_bitmap = [1]*MAX_NUM_PROCESSES
        self.epoch_offsets = [0]*MAX_NUM_PROCESSES
        self.counter_bitmap = [0]*MAX_NUM_PROCESSES
        self.counters = [0]*MAX_NUM_PROCESSES
        self.pid = pid
        self.epoch_time = epsilon / 4

    def __repr__(self) -> str:
        
        return "HVC(origin_timestamp={pwc}, epoch_offsets={epochs}, counters={counters}".format(pwc = self.pwc.as_int(), epochs = self.epoch_offsets, counters = self.counters)
    
    def update_offsets(self, prev_clock: int):

        for offset in range(len(self.epoch_offsets)):

            if self.epoch_bitmap[offset] == 1:

                offset_time = prev_clock + self.epoch_offsets[offset]

                if (self.pwc.as_int() - offset_time) > self.epsilon:
                    self.epoch_bitmap[offset] = 0
                else:
                    self.epoch_offsets[offset] = self.pwc.as_int() - offset_time

    def advance(self, physical_input: int, pt_only: bool = False) -> None:

        prev_clock = self.pwc.as_int()
        self.pwc.advance(physical_input=physical_input, pt_only=pt_only)
        new_clock = self.pwc.as_int()
        if new_clock - prev_clock > self.epoch_time:
            # New epoch
            self.update_offsets(prev_clock)
            self.counters = [0]*MAX_NUM_PROCESSES
        else:
            self.counters[self.pid] += 1
            self.counter_bitmap[self.pid] = 1

    def merge(self, other: 'HVC') -> None:
        
        prev_clock = self.pwc.as_int()
        self.pwc.merge(other.pwc)
        new_clock = self.pwc.as_int()
        if new_clock - prev_clock > self.epoch_time:
            # New epoch
            self.update_offsets(prev_clock)
            self.counters = [0]*MAX_NUM_PROCESSES
        else:
            self.counters[self.pid] += 1
            self.counter_bitmap[self.pid] = 1
