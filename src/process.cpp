#include <vector>
#include <queue>
#include <algorithm>
#include "process.h"

using namespace std;

bool CompareByRecvTimes(Message a, Message b)
{
    return a.GetRecvTime() < b.GetRecvTime();
}

Message Process::CreateMessage(int recv_time, string body)
{
    Message m = Message(recv_time, hvc_clock, body);
    return m;
}
void Process::PushMsg(Message m)
{
    msg_queue.push(m);
}
void Process::GetMessages(int phy_clock)
{

    while (!msg_queue.empty() && msg_queue.front().GetRecvTime() <= phy_clock)
    {
        Message front = msg_queue.front();
        if (front.GetRecvTime() <= phy_clock)
        {
            msg_queue.pop();
            proc_queue.push_back(front);
        }
    }
    sort(proc_queue.begin(), proc_queue.end(), CompareByRecvTimes);
}   

void Process::ProcessMessages()
{
    for(int i = 0; i < proc_queue.size(); i++)
    {
        hvc_clock.Recv(proc_queue[i].GetMsgClock(), CalculateEpoch(phy_clock));
    }
    proc_queue.clear();
}

int Process::CalculateEpoch(int phy_clock)
{
    return phy_clock / interval;
}