/*
	process.h - class Process and class Message
*/

#ifndef PROCESS_H
#define PROCESS_H

#include <set>
#include <vector>
#include <queue>   // std::queue
#include <numeric> // for accumulate()

#include "hvc.h"

using namespace std;

/*
  External global variables
*/

extern int number_of_processes;
extern double alpha;
extern distance_matrix D;

/*
  message - structure of a message communicated between processes
*/

struct message
{
	message(int arrival_time, const HVC &sender_at, int sender_id) : arrival_time(arrival_time), sender_at(sender_at), sender_id(sender_id) {}

	int arrival_time;
	HVC sender_at;
	int sender_id;

	// define the operator <() which is used to order messages in mailbox (i.e. set<message>)
	bool operator<(const message &rhs) const
	{
		if (arrival_time != rhs.arrival_time)
			return arrival_time < rhs.arrival_time;
		return sender_id < rhs.sender_id;
	}
};

/*
  Process - class for a process/node in a distributed system
*/

class Process
{
private:
	bool time_locked = 0; // to control RefreshClock() from updating multiple
						  // times within one physical clock cycle.
						  // When time_locked = false, RefreshClock() will update augmented_time
						  // when time_locked = true, RefreshClock() does nothing

	int my_time_now = epsilon; // time of process at current physical clock cycle

	int id = -1;		   // process' ID.
	set<message> mail_box; // collection of messages a process received, ordered by time of reception
	HVC augmented_time;	   // hybrid vector clock
	vector<int> size_of_at_history;
	int interval; 			// Interval

	// private methods
	void CountActiveSize();
	int GetTimeNow() const;
	void RefreshClock();

public:
	// Constructor
	Process() {}

	Process(int id) : id(id)
	{
		// initialize augmented_time as an HVC with all elements have value -1*epsilon - 1
		// and active_size = 1
		augmented_time = HVC(vector<int>(number_of_processes, -1 * epsilon - 1), vector<int>(number_of_processes, 0), 1, id);

		my_time_now = epsilon;
	}

	// gets and sets

	int GetId() { return id; }

	// methods for message communication
	void SendMsg(Process &k);
	void ReceiveMsg();
	void BroadcastMsg(vector<Process> &vp);
	void PushMsg(const message &m);

	// http://duramecho.com/ComputerInformation/WhyHowCppConst.html
	// const to avoid modifying class members
	double GetSumSizeActive() const
	{
		return accumulate(begin(size_of_at_history), end(size_of_at_history), 0);
	}

	double GetAVGSizeActive() const
	{
		return GetSumSizeActive() / (int)size_of_at_history.size();
	}

	int GetNumEvents() const
	{
		return size_of_at_history.size();
	}

	void UnlockTime() { time_locked = false; }

	int CountActiveSizeNow();

	int setInterval(int systemInterval) {
		interval = systemInterval;
	}
	void Merge(HVC m_hvc);

	int CalculateEpoch();
};

#endif
