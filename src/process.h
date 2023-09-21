#include <vector>
#include <queue>
#include <fstream>
#include "message.h"

using namespace std;

class Process
{
private:
    HVC hvc_clock;
    vector<Message> msg_queue;
    int pid;
    float phy_clock;
    int interval;

public:
    Process() {}
    Process(int somePid, int epsilon, int someInterval, int num_procs)
    {
        pid = somePid;
        hvc_clock = HVC(epsilon, someInterval, pid, num_procs);
        hvc_clock.SetEpoch(epsilon);
        interval = someInterval;
        phy_clock = epsilon*interval;
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
        // hvc_clock.Tick(CalculateEpoch(phy_clock));
    }

    void Send()
    {
        hvc_clock.SendLocal(CalculateEpoch(phy_clock));
    }

    int CalculateEpoch(int phy_clock);

    Message CreateMessage(int recv_time, string body);
    
    void PushMsg(Message m);

    void GetMessages(int phy_clock);

    float ProcessMessages();

    void PrintMessages(vector<Message> msg_queue);
    
};