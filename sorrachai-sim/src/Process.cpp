
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>

using namespace std;

#include "Process.h"

/* send message to a set of processes */

void Process::BroadcastMsg(vector<Process> &vp)
{
	for (int k = 0; k < number_of_processes; k++)
	{
		if (k != id)
			SendMsg(vp[k]);
	}
}

/* send message to a process */

void Process::SendMsg(Process &k)
{

	my_time_now = GetTimeNow();
	RefreshClock();

	if (Utility::GetRandomNumberInRange(1, 100) * 1.0 <= alpha * 100)
	{
		// D is distance matrix
		int distance = D[id][k.id];

		int time_delay_msg = 0;
		for (int i = 0; i < distance; i++)
		{
			time_delay_msg += PhysicalTime::GetDelayMessageTime();
		}
		CountActiveSize();
		int as = CountActiveSizeNow();
		augmented_time.setActiveSize(as);
		k.PushMsg(message(time_delay_msg + absolute_time, augmented_time, id));
	}
}

void Process::CountActiveSize()
{

	// Duong: we only push size of active count into history
	//        after we have passed the uncertainty period (i.e. epsilon)
	if (absolute_time > epsilon)
	{
		int count_active = 0;
		for (int i = 0; i < augmented_time.getOffsets().size(); i++)
		{
			if (augmented_time.getOffsets()[i] > my_time_now - epsilon)
			{
				count_active++;
			}
		}
		size_of_at_history.push_back(count_active);
	}
}

int Process::CountActiveSizeNow()
{

	my_time_now = GetTimeNow();

	RefreshClock();
	int count_active = 0;
	for (int i = 0; i < augmented_time.getOffsets().size(); i++)
	{
		if (augmented_time.getOffsets()[i] > my_time_now - epsilon)
		{
			count_active++;
		}
	}
	return count_active;
}

void Process::ReceiveMsg()
{

	if (!mail_box.empty() && begin(mail_box)->arrival_time <= PhysicalTime::GetAbsoluteTime())
	{
		while (!mail_box.empty() && begin(mail_box)->arrival_time <= PhysicalTime::GetAbsoluteTime())
		{
			auto m_itr = begin(mail_box);
			HVC m_hvc = m_itr->sender_at;
			Merge(m_hvc);
			mail_box.erase(m_itr);
		}
		my_time_now = GetTimeNow();
		RefreshClock();
		CountActiveSize();
	}
}

void Process::PushMsg(const message &m)
{
	mail_box.insert(m);
}

int Process::GetTimeNow() const
{

	if (time_locked)
		return my_time_now;

	return max(my_time_now + 1, PhysicalTime::GetTimeNow());
}

// RefreshClock is called whenever process sends or receives messages

void Process::RefreshClock()
{

	if (time_locked)
		return;
	for (int i = 0; i < (int)augmented_time.getOffsets().size(); i++)
	{
		if (i == id)
			continue;
		if (augmented_time.getOffsets()[i] < my_time_now - epsilon)
		{
			augmented_time.setHVCOffset(my_time_now - epsilon, i);
		}
	}

	augmented_time.setHVCOffset(my_time_now, id);

	time_locked = true;
}

int Process::CalculateEpoch()
{
	return (GetTimeNow() / interval);
}

void Process::Merge(HVC m_hvc) 
{
	int new_max_epoch = max(augmented_time.getEpoch(), CalculateEpoch());
	new_max_epoch = max(new_max_epoch, m_hvc.getEpoch());

	// If the message is in the same epoch
	if(new_max_epoch == augmented_time.getEpoch() && new_max_epoch == m_hvc.getEpoch())
	{
		for(int i = 0; i < augmented_time.getCounters().size(); i++)
		{
			augmented_time.getCounters()[i] = max(augmented_time.getCounters()[i], m_hvc.getCounters()[i]);
		}
		for(int i = 0; i < augmented_time.getOffsets().size(); i++)
		{
			augmented_time.getOffsets()[i] = max(augmented_time.getOffsets()[i], m_hvc.getOffsets()[i]);
		}
		augmented_time.getCounters()[augmented_time.getId()]++;
		augmented_time.getOffsets()[m_hvc.getId] = 0;
	}

	// If the message is lagging
	else if(new_max_epoch == augmented_time.getEpoch())
	{
		augmented_time.getCounters()[augmented_time.id]++;

		int msg_offset = augmented_time.getEpoch() - m_hvc.getEpoch();
		for(int i = 0; i < augmented_time.getOffsets().size(); i++)
		{
			if(m_hvc.getOffsets()[i] + msg_offset < epsilon)
			{
				augmented_time.getOffsets()[i] = max(augmented_time.getOffsets()[i], m_hvc.getOffsets()[i] + msg_offset);
			}

		}
	}

        # If the message is lagging
        elif new_max_epoch == self.max_epoch:

            if (self.counters[self.pid]):
                self.counters[self.pid] += 1
            else:
                self.counters[self.pid] = 1

            msg_offset = self.max_epoch - m.max_epoch
            for key in m.offsets.keys() - self.offsets.keys():
                if m.offsets[key] + msg_offset < self.epsilon:
                    self.offsets[key] = m.offsets[key] + msg_offset

            if msg_offset < self.epsilon:
                for key in m.counters.keys() - self.counters.keys():
                    self.counters[key] = m.counters[key]

            if msg_offset < self.epsilon:
                self.offsets[m.pid] = msg_offset
            else:
                del self.offsets[m.pid]

        # If the message is leading
        elif new_max_epoch == m.max_epoch:

            msg_offset = m.max_epoch - self.max_epoch

            for key in m.counters.keys():
                self.counters[key] = m.counters[key]

            for key in self.offsets.keys():
                if self.offsets[key] + msg_offset < self.epsilon:
                    self.offsets[key] += msg_offset

            for key in m.offsets.keys():
                self.offsets[key] = m.offsets[key]

            self.max_epoch = m.max_epoch

        else:
            self.advance(phy_clock=phy_clock)

        self.offsets[self.pid] = 0
}