#include <string>
#include "rc.h"

using namespace std;

class MPI_MESSAGE
{
    RC rc_clock;
    string message;

public:
    Message() {}
    Message(RC rc, string body)
    {
        rc_clock = rc;
        msg = body;
    }
    string GetMessage()
    {
        return msg;
    }
    string GetMsgClock()
    {
        return rc_clock.Serialize();
    }
};