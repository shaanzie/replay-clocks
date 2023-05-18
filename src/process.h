#include <vector>
#include <queue>
#include <fstream>
#include "message.h"

using namespace std;

class Process
{
private:
    HVC hvc_clock;
    queue<Message> msg_queue;
    vector<Message> proc_queue;
    int pid;
    int phy_clock;
    int interval;

public:
    Process() {}
    Process(int somePid, int epsilon, int someInterval, int num_procs)
    {
        pid = somePid;
        hvc_clock = HVC(epsilon, someInterval, pid, num_procs);
        interval = someInterval;
        phy_clock = 0;
    }
    HVC GetClock()
    {
        return hvc_clock;
    }
    int GetPID()
    {
        return pid;
    }
    int GetPhyClock()
    {
        return phy_clock;
    }

    void Tick()
    {
        phy_clock++;
        hvc_clock.Shift(CalculateEpoch(phy_clock));
    }

    int CalculateEpoch(int phy_clock);

    Message CreateMessage(int recv_time, string body);
    
    void PushMsg(Message m);

    void GetMessages(int phy_clock);

    void ProcessMessages();
    
};