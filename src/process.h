#include <vector>
#include <queue>
#include <fstream>
#include "message.h"

using namespace std;

class Process
{
private:
    RC rc_clock;
    vector<Message> msg_queue;
    int pid;
    float phy_clock;
    int interval;

public:
    Process() {}
    Process(int somePid, int epsilon, int someInterval, int num_procs)
    {
        pid = somePid;
        rc_clock = RC(pid, num_procs);
        rc_clock.SetHLC(epsilon);
        interval = someInterval;
        phy_clock = epsilon*interval;
    }
    RC GetClock()
    {
        return rc_clock;
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
        // RC_clock.Tick(CalculateEpoch(phy_clock));
    }

    void Send()
    {
        rc_clock.SendLocal(CalculateEpoch(phy_clock));
    }

    int CalculateEpoch(int phy_clock);

    Message CreateMessage(int recv_time, string body);
    
    void PushMsg(Message m);

    void GetMessages(int phy_clock);

    float ProcessMessages();

    void PrintMessages(vector<Message> msg_queue);
    
};