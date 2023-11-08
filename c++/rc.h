// RC header

#include <vector>
#include <iostream>
#include <bitset>
#include <sstream>
#include <string>
#include "config.h"


using namespace std;

class RC
{

private:
    int hlc;
    bitset<NUM_PROCS> offset_bitmap;
    bitset<MAX_OFFSET_BITS*MAX_OFFSETS> offsets;
    int counters;
    int pid;

public:

    RC() {}

    RC(int process_id, int num_procs)
    {
        hlc = 0;
        pid = process_id;
        offset_bitmap = 0;
        counters = 0;
        
        // Setting the bit of the pid to show that we have an offset for it
        offset_bitmap[pid] = 1; 

        // offsets will be all 0 by default, which is the right offset for the first process
        
    }

    // Copy constructor (deep copy)
    RC(const RC &other)
    {
        pid = other.pid;
        hlc = other.hlc;
        offset_bitmap = other.offset_bitmap;
        counters = other.counters;
        offsets = other.offsets;
    }

    // Assignment operator (deep copy)
    RC &operator=(const RC &other)
    {
        if (this != &other)
        {
            pid = other.pid;
            hlc = other.hlc;
            offset_bitmap = other.offset_bitmap;
            counters = other.counters;
            offsets = other.offsets;
        }
        return *this;
    }


    // Getters and Setters
    int GetPID()
    {
        return pid;
    }

    int GetHLC()
    {
        return hlc;
    }

    bitset<NUM_PROCS> GetBitmap() 
    {
        return offset_bitmap;
    }

    bitset<MAX_OFFSET_BITS*MAX_OFFSETS> GetOffsets()
    {
        return offsets;
    }

    int GetCounters()
    {
        return counters;
    }

    void setPID(int pid)
    {
        pid = pid;
    }

    void SetHLC(int new_hlc)
    {
        hlc = new_hlc;
    }

    void SetOffsets(bitset<MAX_OFFSET_BITS*MAX_OFFSETS> offset_vec)
    {
        offsets = offset_vec;
    }

    void SetCounters(int counter_map)
    {
        counters = counter_map;
    }

    void SetOffsetBitmap(bitset<NUM_PROCS> bitmap)
    {
        offset_bitmap = bitmap;
    }

    string Serialize()
    {
        std::ostringstream ss;
        ss << hlc << " " << offset_bitmap << " " << offsets << " " << counters;
        return ss.str();
    }

    void Deserialize(string RC_str)
    {
        istringstream ss(RC_str);
        ss >> hlc >> offset_bitmap >> offsets >> counters;
    }

    // Send/Local event (advance clock)
    void SendLocal(int phy_clock_hlc);

    // Recv event (merge clock)
    void Recv(RC m_RC, int phy_clock_hlc);

    // Shift operation
    void Shift(int phy_clock_hlc);

    // Helper
    void MergeSameEpoch(RC m_RC);

    bool EqualOffset(RC a);

    int GetOffsetAtIndex(int index);

    void SetOffsetAtIndex(int index, int new_offset);

    void RemoveOffsetAtIndex(int index);

    bool IsEqual(RC f);

    void PrintClock();

    int OffsetSize();

    int CounterSize();

    int ClockSize();

};