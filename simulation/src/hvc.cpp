#include <vector>
#include <iostream>
#include <math.h>
#include "hvc.h"

using namespace std;


void HVC::Shift(int new_epoch)
{
    int bitmap = offset_bitmap;
    while(bitmap > 0)
    {
        int pos = log2((~(bitmap ^ (~(bitmap - 1))) + 1) >> 1);
        // cout << pos << endl;
        offsets[pos] = min(new_epoch - (epoch - offsets[pos]), epsilon);
        if(offsets[pos] >= epsilon)
        {
            offset_bitmap = offset_bitmap & ~(1 << pos); 
        }
        bitmap = bitmap & (bitmap - 1);
    }
    epoch = new_epoch;
}

void HVC::MergeSameEpoch(HVC m_hvc)
{
    
    offset_bitmap = offset_bitmap | m_hvc.offset_bitmap;
    int bitmap = offset_bitmap;
    while(bitmap > 0)
    {
        int pos = log2((~(bitmap ^ (~(bitmap - 1))) + 1) >> 1);
        offsets[pos] = min(offsets[pos], m_hvc.offsets[pos]);
        if(offsets[pos] >= epsilon)
        {
            offset_bitmap = offset_bitmap & ~(1 << pos); 
        }
        bitmap = bitmap & (bitmap - 1);
    }
}

void HVC::SendLocal(int phy_clock_epoch)
{

    // printClock();

    // cout << "After send: " << endl;

    int new_epoch = max(epoch, phy_clock_epoch);
    int new_offset = new_epoch - phy_clock_epoch;
    // Make sure this case does not happen
    if (new_epoch == epoch && offsets[pid] <= new_offset)
    {
        counters[pid]++;
    }
    else if(new_epoch == epoch)
    {
        offsets[pid] = min(new_offset, offsets[pid]);
        for(int i = 0; i < counters.size(); i++)
        {
            counters[i] = 0;
        }
    }
    else
    {
        
        for(int i = 0; i < counters.size(); i++)
        {
            counters[i] = 0;
        }
        Shift(new_epoch);
        offsets[pid] = min(new_offset, epsilon); // Is this always 0
    }

    // printClock();
    // cout << endl;   
}

void HVC::Recv(HVC m_hvc, int phy_clock_epoch)
{

    // cout << "--------------------------------------------------------------------------------------------" << endl;
    // cout << "Recv op" << endl;
    // cout << "Process: " << *this << endl;
    // cout << "Message: " << m_hvc << endl;
    

    int new_epoch = max(epoch, m_hvc.epoch);
    new_epoch = max(new_epoch, phy_clock_epoch);

    // cout << "New epoch " << new_epoch << endl;

    HVC a = *this;
    HVC b = m_hvc;

    a.Shift(new_epoch);

    // cout << "A shifted: " << a << endl;

    b.Shift(new_epoch);

    // cout << "B shifted: " << b << endl;

    a.MergeSameEpoch(b);

    // cout << "A merged: " << a << endl;


    if(EqualOffset(a) && m_hvc.EqualOffset(a))
    {
        for(int i = 0; i < a.counters.size(); i++)
        {
            a.counters[i] = max(a.counters[i], m_hvc.counters[i]);
        }
        a.counters[pid]++;
    }
    else if(EqualOffset(a) && !m_hvc.EqualOffset(a))
    {
        a.counters[pid]++;
    }
    else if(!EqualOffset(a) && m_hvc.EqualOffset(a))
    {
        a.counters = m_hvc.counters;
        a.counters[pid]++;
    }
    else
    {
        for(int i = 0; i < a.counters.size(); i++)
        {
            a.counters[i] = 0;
        }
    }

    *this = a;

    // cout << "Final: " << *this << endl;
    // cout << "--------------------------------------------------------------------------------------------" << endl;

}



bool HVC::EqualOffset(HVC a)
{
    if(a.epoch != epoch || a.offsets != offsets)
    {
        return false;
    }
    return true;
}

bool HVC::IsEqual(HVC &f)
{
    return (offsets == f.offsets && counters == f.counters && epoch == f.epoch);
}

vector<float> HVC::AsList()
{
    vector<float> res;
    for (int i = 0; i < offsets.size(); i++)
    {
        res.push_back((float)epoch - offsets[i]);
    }
    for (int i = 0; i < counters.size(); i++)
    {
        res[i] += (float)counters[i] / 10;
    }
    return res;
}

bool HVC::HappensBefore(HVC &f)
{

    vector<float> e_list = AsList();
    vector<float> f_list = AsList();
    bool result;
    int flag = 0;

    for (int i = 0; i < e_list.size(); i++)
    {
        if (e_list[i] > f_list[i])
        {
            result = false;
        }
        if (e_list[i] < f_list[i])
        {
            flag = 1;
        }
    }
    if (flag == 1)
    {
        result = true;
    }
    else
    {
        result = false;
    }
    if (result == false)
    {
        cout << endl
             << "e:";
        for (auto i : e_list)
        {
            cout << i << " ";
        }
        cout << endl
             << "f:";
        for (auto i : f_list)
        {
            cout << i << " ";
        }
    }
    return result;
}

void HVC::Tick(int someEpoch)
{
    Shift(someEpoch);
    offsets[pid] = 0;
}