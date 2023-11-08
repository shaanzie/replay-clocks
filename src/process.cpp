#include <vector>
#include <queue>
#include <chrono>
#include <algorithm>
#include "process.h"

using namespace std;

bool CompareByRecvTimes(Message a, Message b)
{
    return a.GetRecvTime() < b.GetRecvTime();
}

Message Process::CreateMessage(int recv_time, string body)
{
    Message m = Message(recv_time, rc_clock, body);
    return m;
}
void Process::PushMsg(Message m)
{
    msg_queue.push_back(m);
    sort(msg_queue.begin(), msg_queue.end(), CompareByRecvTimes);
    // PrintMessages(msg_queue);
}

float Process::ProcessMessages()
{

    // cout << "Messages for process " << pid << " at pt " << phy_clock << endl;
    // PrintMessages(msg_queue);
    int msg_queue_size = msg_queue.size() + 1;
    float recv_time = 0;

    for(int i = 0; i < msg_queue.size(); i++)
    {
        if(msg_queue[i].GetRecvTime() <= phy_clock)
        {
            
            auto start = chrono::high_resolution_clock::now();

            rc_clock.Recv(msg_queue[i].GetMsgClock(), CalculateEpoch(phy_clock));

            auto stop = chrono::high_resolution_clock::now();
            auto recv_duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);

            recv_time += recv_duration.count();

            msg_queue.erase(msg_queue.begin() + i);
        }
    }

    return recv_time / msg_queue_size;
}

int Process::CalculateEpoch(int phy_clock)
{
    return phy_clock / interval;
}