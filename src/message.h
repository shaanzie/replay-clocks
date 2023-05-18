#include <vector>
#include <string>
#include <iostream>
#include "hvc.h"

using namespace std;

class Message
{
private:
    string msg;
    HVC m_hvc;
    int recv_time;

public:
    Message() {}
    Message(int clock, HVC hvc, string body)
    {
        recv_time = clock;
        m_hvc = hvc;
        msg = body;
    }
    string GetMessage()
    {
        return msg;
    }
    HVC GetMsgClock()
    {
        return m_hvc;
    }
    int GetRecvTime()
    {
        return recv_time;
    }
    friend std::ostream &operator<<(std::ostream &os, const Message &obj)
    {
        cout << "Message HVC: " << endl << obj.m_hvc;
        cout << "Recv time: " << obj.recv_time;
        return os;
    }
};