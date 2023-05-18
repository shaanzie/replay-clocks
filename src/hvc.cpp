#include <vector>
#include "hvc.h"

using namespace std;

void HVC::Shift(int new_epoch)
{
    for (int i = 0; i < offsets.size(); i++)
    {
        offsets[i] = min(new_epoch - (epoch - offsets[i]), epsilon);
    }
    epoch = new_epoch;
}

void HVC::SendLocal(int phy_clock_epoch)
{
    int new_epoch = max(epoch, phy_clock_epoch);
    if (new_epoch == epoch)
    {
        counters[pid]++;
    }
    else
    {
        
        for(int i = 0; i < counters.size(); i++)
        {
            counters[i] = 0;
        }
        Shift(phy_clock_epoch);
        offsets[pid] = 0;
    }
}

void HVC::Recv(HVC &m_hvc, int phy_clock_epoch)
{

    int new_epoch = max(epoch, m_hvc.epoch);
    new_epoch = max(new_epoch, phy_clock_epoch);

    // If physical clock leads the clocks recieved
    if (new_epoch == phy_clock_epoch && phy_clock_epoch != m_hvc.epoch && phy_clock_epoch != epoch)
    {
        SendLocal(phy_clock_epoch);
    }

    // Both message and self are in the same epoch
    if (m_hvc.epoch == epoch)
    {
        MergeSameEpoch(m_hvc);
    }

    // Message is lagging
    else if (new_epoch == epoch)
    {
        m_hvc.Shift(epoch);
        MergeSameEpoch(m_hvc);
        for(int i = 0; i < counters.size(); i++)
        {
            counters[i] = 0;
        }
    }

    // Message is leading
    else if (new_epoch == m_hvc.epoch)
    {
        Shift(m_hvc.epoch);
        MergeSameEpoch(m_hvc);
        for(int i = 0; i < counters.size(); i++)
        {
            counters[i] = 0;
        }
    }
}

void HVC::MergeSameEpoch(HVC &m_hvc)
{
    for (int i = 0; i < offsets.size(); i++)
    {
        offsets[i] = min(offsets[i], m_hvc.offsets[i]);
    }
    for (int i = 0; i < counters.size(); i++)
    {
        counters[i] = max(counters[i], m_hvc.counters[i]);
    }
    counters[pid]++;
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