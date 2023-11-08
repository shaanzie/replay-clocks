#include <vector>
#include <string>
#include <iostream>
#include "rc.h"

using namespace std;

class Message
{
private:
    string msg;
    RC m_RC;
    int recv_time;

public:
    Message() {}
    Message(int clock, RC rc, string body)
    {
        recv_time = clock;
        m_RC = rc;
        msg = body;
    }
    string GetMessage()
    {
        return msg;
    }
    RC GetMsgClock()
    {
        return m_RC;
    }
    int GetRecvTime()
    {
        return recv_time;
    }
};