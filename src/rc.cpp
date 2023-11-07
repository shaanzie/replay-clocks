#include <vector>
#include <iostream>
#include <math.h>
#include "rc.h"
#include "config.h"

using namespace std;


void RC::SendLocal(int phy_clock_hlc)
{

    int new_hlc = max(hlc, phy_clock_hlc);
    int new_offset = new_hlc - phy_clock_hlc;

    int offset_at_pid = GetOffsetAtIndex(pid);

    // Make sure this case does not happen
    if (new_hlc == hlc && offset_at_pid <= new_offset)
    {
        counters++;
    }
    else if(new_hlc == hlc)
    {
        new_offset = min(new_offset, offset_at_pid);
        SetOffsetAtIndex(pid, new_offset);
        counters = 0;
    }
    else
    {    
        counters = 0;
        Shift(new_hlc);
        SetOffsetAtIndex(pid, 0);
    }
}

void RC::Recv(RC m_RC, int phy_clock_hlc)
{
    int new_hlc = max(hlc, m_RC.hlc);
    new_hlc = max(new_hlc, phy_clock_hlc);

    RC a = *this;
    RC b = m_RC;

    a.Shift(new_hlc);

    b.Shift(new_hlc);

    a.MergeSameEpoch(b);

    if(EqualOffset(a) && m_RC.EqualOffset(a))
    {
        a.counters = max(a.counters, m_RC.counters);
        a.counters++;
    }
    else if(EqualOffset(a) && !m_RC.EqualOffset(a))
    {
        a.counters++;
    }
    else if(!EqualOffset(a) && m_RC.EqualOffset(a))
    {
        a.counters = m_RC.counters;
        a.counters++;
    }
    else
    {
        a.counters = 0;
    }

    *this = a;
}

void RC::Shift(int new_hlc)
{
    int bitmap = offset_bitmap.to_ulong();
    int index = 0;
    while(bitmap > 0)
    {

        int offset_at_index = GetOffsetAtIndex(index);

        int pos = log2((~(bitmap ^ (~(bitmap - 1))) + 1) >> 1);

        int new_offset = min(new_hlc - (hlc - offset_at_index), EPSILON);

        if(new_offset >= EPSILON)
        {    
            offset_bitmap[pos] = 0;
            RemoveOffsetAtIndex(index);
        }
        else
        {
            SetOffsetAtIndex(index, new_offset);
        }

        bitmap = bitmap & (bitmap - 1);

        index++;
    }
    hlc = new_hlc;
}

void RC::MergeSameEpoch(RC m_RC)
{
    
    offset_bitmap = offset_bitmap | m_RC.offset_bitmap;

    int bitmap = offset_bitmap.to_ulong();
    int index = 0;

    while(bitmap > 0)
    {
        int pos = log2((~(bitmap ^ (~(bitmap - 1))) + 1) >> 1);
        
        int new_offset = min(GetOffsetAtIndex(pos), m_RC.GetOffsetAtIndex(pos));
        SetOffsetAtIndex(index, new_offset);

        if(GetOffsetAtIndex(pos) >= EPSILON)
        {
            offset_bitmap[pos] = 0;
            RemoveOffsetAtIndex(index); 
        }
        bitmap = bitmap & (bitmap - 1);

        index++;
    }
}


bool RC::EqualOffset(RC a)
{
    if(a.hlc != hlc || a.offset_bitmap != offset_bitmap || a.offsets != offsets)
    {
        return false;
    }
    return true;
}

// Function to extract k bits from p position
// and returns the extracted value as integer
int extract(int number, int k, int p)
{
    return (((1 << k) - 1) & (number >> (p)));
}

int RC::GetOffsetAtIndex(int index)
{
    
    // cout << endl << "Extract " << MAX_OFFSET_BITS << " bits from position " << MAX_OFFSET_BITS*index << endl;

    bitset<MAX_OFFSET_BITS> offset(extract(offsets.to_ulong(), MAX_OFFSET_BITS, MAX_OFFSET_BITS*index));
    
    // cout << "Offset: " << offset << endl;

    return offset.to_ulong();

}

void RC::SetOffsetAtIndex(int index, int new_offset)
{
    // Convert new offset to bitset and add bitset at appropriate position
    
    bitset<MAX_OFFSET_BITS> offset(new_offset);

    bitset<MAX_OFFSET_BITS*MAX_OFFSETS> res(extract(offsets.to_ulong(), MAX_OFFSET_BITS*index, 0));

    res |= offset.to_ulong() << index*MAX_OFFSET_BITS;

    bitset<MAX_OFFSET_BITS*MAX_OFFSETS> lastpart(extract(offsets.to_ulong(), MAX_OFFSET_BITS*MAX_OFFSETS - (MAX_OFFSET_BITS*(index + 1)), MAX_OFFSET_BITS*(index + 1)));

    res |= lastpart << ((index+1)*MAX_OFFSET_BITS);

    offsets = res;

}

void RC::RemoveOffsetAtIndex(int index)
{
    // Remove and squash the bitset of given index through index + 4
    bitset<MAX_OFFSET_BITS*MAX_OFFSETS> res(extract(offsets.to_ulong(), MAX_OFFSET_BITS*index, 0));
    
    bitset<MAX_OFFSET_BITS*MAX_OFFSETS> lastpart(extract(offsets.to_ulong(), MAX_OFFSET_BITS*MAX_OFFSETS - (MAX_OFFSET_BITS*(index + 1)), MAX_OFFSET_BITS*(index + 1)));

    res |= lastpart << ((index+1)*MAX_OFFSET_BITS);

    offsets = res;

}

bool RC::IsEqual(RC f)
{
    if(hlc != f.GetHLC() || offset_bitmap != f.GetBitmap() || offsets != f.GetOffsets() || counters != f.GetCounters())
    {
        return false;
    }
    return true;
}

void RC::PrintClock()
{
    cout << "HLC: " << hlc << endl;
    cout << "Offset bitmap: " << offset_bitmap << endl;
    cout << "Offsets in binary: " << offsets << endl;
    cout << "Offsets: " << endl;

    int index = 0;
    int bitmap = offset_bitmap.to_ulong();
    while(bitmap > 0)
    {

        int offset_at_index = GetOffsetAtIndex(index);

        int pos = log2((~(bitmap ^ (~(bitmap - 1))) + 1) >> 1);

        cout << pos << ":" << offset_at_index << ",";
        
        bitmap = bitmap & (bitmap - 1);

        index ++; 

    }
    cout << endl << "Counters: " << counters << endl;
}