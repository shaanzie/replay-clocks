import datetime as dt

MAX_NUM_PROCESSES=64

class HVC: 

    def __init__(self, times: list, epsilon: int, pid: int) -> None:
        
        self.max_epoch = max(times)
        self.times = times
        self.epsilon = epsilon
        self.counters = [0]*MAX_NUM_PROCESSES
        self.offsets = [-epsilon]*MAX_NUM_PROCESSES
        self.pid = pid
        for idx in range(len(times)):
            self.offsets[idx] = min(self.max_epoch - times[idx], epsilon)

    def __repr__(self) -> str:
        
        return '\nHVC(max_epoch={max_epoch}, \ncounters={counters}, \noffsets={offsets}'.format(
            max_epoch = self.max_epoch, 
            counters = self.counters, 
            offsets = self.offsets
        )

    def as_list(self) -> list:

        vector_clock = [0]*MAX_NUM_PROCESSES
        for pid in range(MAX_NUM_PROCESSES):
            vector_clock[pid] = self.max_epoch + self.offsets[pid] + (self.counters[pid] / MAX_NUM_PROCESSES)
        return vector_clock

    def __eq__(self, other: 'HVC') -> bool:
        return self.as_list() == other.as_list()

    def __ne__(self, other: 'HVC') -> bool:
        return self.as_list() != other.as_list()

    def __lt__(self, other: 'HVC') -> bool:
        self_vc = self.as_list()
        other_vc = other.as_list()
        for idx in range(MAX_NUM_PROCESSES):
            if self_vc[idx] >= other_vc[idx]:
                return False

    def __le__(self, other: 'HVC') -> bool:
        self_vc = self.as_list()
        other_vc = other.as_list()
        for idx in range(MAX_NUM_PROCESSES):
            if self_vc[idx] > other_vc[idx]:
                return False

    def __gt__(self, other: 'HVC') -> bool:
        self_vc = self.as_list()
        other_vc = other.as_list()
        for idx in range(MAX_NUM_PROCESSES):
            if self_vc[idx] <= other_vc[idx]:
                return False

    def __ge__(self, other: 'HVC') -> bool:
        self_vc = self.as_list()
        other_vc = other.as_list()
        for idx in range(MAX_NUM_PROCESSES):
            if self_vc[idx] < other_vc[idx]:
                return False
    
    def find_system_epoch(self):

        return dt.datetime.now(dt.timezone.utc).time()
        
    def advance(self, phy_clock: int):
        
        # system_epoch = self.find_system_epoch()
        new_max_epoch = max(self.max_epoch, phy_clock)

        if(new_max_epoch == self.max_epoch):
            self.counters[self.pid] += 1
        
        else:
            self.counters = [0]*MAX_NUM_PROCESSES
            for idx in range(MAX_NUM_PROCESSES):
                pid_time = self.max_epoch + self.offsets[idx]
                self.offsets[idx] = min(self.max_epoch - pid_time, self.epsilon)

        self.max_epoch = new_max_epoch
        
    def merge(self, m: 'HVC', phy_clock: int):

        # system_epoch = self.find_system_epoch()
        new_max_epoch = max(self.max_epoch, phy_clock, m.max_epoch)

        if new_max_epoch == self.max_epoch:
            self.counters[self.pid] += 1

        elif new_max_epoch == m.max_epoch:
            self.max_epoch = m.max_epoch
            self.times = m.times
            self.counters = m.counters
            self.offsets = m.offsets
            self.counters[self.pid] += 1

        else:
            self.advance(phy_clock=phy_clock)