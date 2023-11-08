// REPCL header

#include <vector>
#include <iostream>

using namespace std;

class REPCL
{

private:
    int pid;
    int epoch;
    vector<int> offsets;
    vector<int> counters;

public:
    int epsilon;
    int interval;

    REPCL() {}

    REPCL(int someEpsilon, int someInterval, int somePID, int num_procs)
    {
        epoch = 0;
        epsilon = someEpsilon;
        interval = someInterval;
        pid = somePID;
        counters = vector<int>(num_procs, 0);
        offsets = vector<int>(num_procs);
        offsets[pid] = 0;
        for (int i = 0; i < num_procs; ++i) {
            offsets[i] = (i == pid) ? 0 : epsilon;
        }
    }

    // Copy constructor (deep copy)
    REPCL(const REPCL &other)
    {
        epoch = other.epoch;
        epsilon = other.epsilon;
        interval = other.interval;
        pid = other.pid;
        offsets = std::vector<int>(other.offsets.begin(), other.offsets.end());
        counters = std::vector<int>(other.counters.begin(), other.counters.end());

    }

    // Assignment operator (deep copy)
    REPCL &operator=(const REPCL &other)
    {
        if (this != &other)
        {
            epoch = other.epoch;
            epsilon = other.epsilon;
            interval = other.interval;
            pid = other.pid;
            offsets = std::vector<int>(other.offsets.begin(), other.offsets.end());
            counters = std::vector<int>(other.counters.begin(), other.counters.end());

        }
        return *this;
    }


    // Getters and Setters

    int GetEpoch()
    {
        return epoch;
    }

    vector<int> GetOffsets()
    {
        return offsets;
    }

    vector<int> GetCounters()
    {
        return counters;
    }

    int GetPID()
    {
        return pid;
    }

    void SetEpoch(int someEpoch)
    {
        epoch = someEpoch;
    }

    void SetOffset(int someOffset, int pid)
    {
        offsets[pid] = someOffset;
    }

    void SetCounter(int someCounter, int pid)
    {
        counters[pid] = someCounter;
    }

    void SetOffsets(vector<int> &someOffsets)
    {
        offsets = someOffsets;
    }

    void SetCounters(vector<int> &someCounters)
    {
        counters = someCounters;
    }

    void printClock()
    {
        cout << "Process: " << pid << endl;
        cout << "Epoch: " << epoch << endl;
        cout << "Offsets: ";
        for (auto i : offsets)
        {
            cout << i << " ";
        }
        cout << endl;
        cout << "Counters: ";
        for (auto i : counters)
        {
            cout << i << " ";
        }
        cout << endl;
    }

    friend std::ostream &operator<<(std::ostream &os, const REPCL &obj)
    {
        cout << "Process: " << obj.pid << endl;
        cout << "Epoch: " << obj.epoch << endl;
        cout << "Offsets: ";
        for (auto i : obj.offsets)
        {
            cout << i << " ";
        }
        cout << endl;
        cout << "Counters: ";
        for (auto i : obj.counters)
        {
            cout << i << " ";
        }
        cout << endl;
        return os;
    }

    int OffsetSize()
    {
        int offsize = 0;
        for (auto i : offsets)
        {
            if (i != epsilon)
            {
                offsize++;
            }
        }
        return offsize;
    }

    int CounterSize()
    {
        int cousize = 0;
        for (auto i : counters)
        {
            if (i != 0)
            {
                cousize++;
            }
        }
        return cousize;
    }

    bool IsEqual(REPCL &f);

    bool HappensBefore(REPCL &f);

    vector<float> AsList();

    // Send/Local event (advance clock)
    void SendLocal(int phy_clock_epoch);

    // Recv event (merge clock)
    void Recv(REPCL m_REPCL, int phy_clock_epoch);

    // Shift operation
    void Shift(int phy_clock_epoch);

    // Helper
    void MergeSameEpoch(REPCL m_REPCL);

    bool EqualOffset(REPCL a);

    void Tick(int someEpoch);
};