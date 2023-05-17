#ifndef CLOCK_H
#define CLOCK_H

/*
  clock.h - classes and functions related to time, clock
            e.g. PhysicalTime, HVC
*/

#include "utility.h"

extern int absolute_time;
extern int epsilon;
extern int delta;
extern int delta_max;

/*
  PhysicalTime - class for simulation of physical time/clock

          absolute_time
    0 -----+-----+-----+-----+-----+-----+-----+-----+-----+-----> run_up_to
            -eps | +eps
            <----+---->
             ^
             |
            PhysicalClock::GetTimeNow() is some randome value within [absolute_time-eps; absolute_time+eps]
             |
             v
            Process::GetTimeNow() = PhysicalClock:GetTimeNow()
             |
             v
            my_time_now = Process:GetTimeNow()

*/

class PhysicalTime
{
public:
    static int GetTimeNow()
    {
        return Utility::GetRandomNumberInRange(absolute_time - epsilon, absolute_time + epsilon);
    }

    static int GetDelayMessageTime()
    {
        return Utility::GetRandomNumberInRange(delta, delta_max);
    }

    static int GetAbsoluteTime()
    {
        return absolute_time;
    }
};

/*
  HVC - class for Hybrid Vector Clock
*/

class HVC
{
private:
    int max_epoch;
    vector<int> offsets;
    vector<int> counters;
    int active_size;
    int id;

public:
    // Constructors

    HVC(){};
    HVC(const HVC &in)
    {
        offsets = in.offsets;
        counters = in.counters;
        active_size = in.active_size;
        id = in.id;
    };
    HVC(const vector<int> &offsets, const vector<int> &counters, int active_size, int id) : offsets(offsets), counters(counters), active_size(active_size), id(id) {}

    // gets and sets

    const int getEpoch()
    {
        return max_epoch;
    }
    vector<int> getOffsets() const
    {
        return offsets;
    }
    const vector<int> getCounters()
    {
        return counters;
    }

    const int getActiveSize()
    {
        return active_size;
    }

    const int getId()
    {
        return id;
    }

    void setHVC(const int someMaxEpoch, const vector<int> &someOffsets, const vector<int> &someCounters)
    {
        max_epoch = someMaxEpoch;
        offsets = someOffsets;
        counters = someCounters;
    }

    void setEpoch(int epoch)
    {
        max_epoch = epoch;
    }

    void setHVCOffset(int time, int pos)
    {
        offsets[pos] = time;
    }

    void setHVCCounter(int count, int pos)
    {
        offsets[pos] = count;
    }

    void setActiveSize(int as)
    {
        active_size = as;
    }

    /*
      happenBefore() - get the logical temporal relationship between this HVC and some other HVC
          return value: 1 if this HVC happens before other HVC
                       -1 if other HVC happens before this HVC
                        0 if concurrent (neither one happens before the other)
    */

    int happenBefore(
        const HVC &they, // the other HVC to be compared
        int size         // number of elements in vector clock = number of processes
    )
    {

        bool youHappenBefore = true;
        bool theyHappenBefore = true;
        int i;

        for (i = 0; i < size; i++)
        {
            if (max_epoch + offsets.at(i) + counters.at(i) < they.max_epoch + they.offsets.at(i) + they.counters.at(i))
                theyHappenBefore = false;
            if (max_epoch + offsets.at(i) + counters.at(i) > they.max_epoch + they.offsets.at(i) + they.counters.at(i))
                youHappenBefore = false;
        }

        /* result
                                 theyHappenBefore
                                    T     F
                                  +----+-----+
                                T | 0  |  1  |
             youHappenBefore      +----+-----+
                                F | -1 |  0  |
                                  +----+-----+
        */

        if (youHappenBefore == theyHappenBefore)
            // concurrent
            return 0;

        if (youHappenBefore)
            // you really happen before other HVC
            return 1;
        else
            // they happens before you
            return -1;
    }
};

#endif
