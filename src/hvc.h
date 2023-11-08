// HVC header

#include <vector>
#include <iostream>

using namespace std;

class HVC
{

private:
    int pid;
    int epoch;
    int offset_bitmap;
    int counter_bitmap;
    vector<int> offsets;
    vector<int> counters;

public:
    int epsilon;
    int interval;

    HVC() {}

    HVC(int someEpsilon, int someInterval, int somePID, int num_procs)
    {
        epoch = 0;
        epsilon = someEpsilon;
        interval = someInterval;
        pid = somePID;
        
        offset_bitmap = 0;
        int bitmask = 1 << pid;
        offset_bitmap = offset_bitmap | bitmask;

        counters = vector<int>(num_procs, 0);
        offsets = vector<int>(num_procs);
        offsets[pid] = 0;
        for (int i = 0; i < num_procs; ++i) {
            offsets[i] = (i == pid) ? 0 : epsilon;
        }
    }

    // Copy constructor (deep copy)
    HVC(const HVC &other)
    {
        epoch = other.epoch;
        epsilon = other.epsilon;
        interval = other.interval;
        pid = other.pid;
        offset_bitmap = other.offset_bitmap;
        counter_bitmap = other.counter_bitmap;
        offsets = std::vector<int>(other.offsets.begin(), other.offsets.end());
        counters = std::vector<int>(other.counters.begin(), other.counters.end());

    }

    // Assignment operator (deep copy)
    HVC &operator=(const HVC &other)
    {
        if (this != &other)
        {
            epoch = other.epoch;
            epsilon = other.epsilon;
            interval = other.interval;
            pid = other.pid;
            offset_bitmap = other.offset_bitmap;
            counter_bitmap = other.counter_bitmap;
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

    void SetBitmap(int bitmap)
    {
        offset_bitmap = bitmap;
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

    friend std::ostream &operator<<(std::ostream &os, const HVC &obj)
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

    bool IsEqual(HVC &f);

    bool HappensBefore(HVC &f);

    vector<float> AsList();

    // Send/Local event (advance clock)
    void SendLocal(int phy_clock_epoch);

    // Recv event (merge clock)
    void Recv(HVC m_hvc, int phy_clock_epoch);

    // Shift operation
    void Shift(int phy_clock_epoch);

    // Helper
    void MergeSameEpoch(HVC m_hvc);

    bool EqualOffset(HVC a);

    void Tick(int someEpoch);
};