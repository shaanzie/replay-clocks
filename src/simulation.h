#include <vector>
#include "process.h"

using namespace std;

class Simulation
{

private:
    int N;
    int E;
    int I;
    int D;
    float A;
    int time;
    vector<Process> vp;
public:
    Simulation() {}
    Simulation(int num_procs, int epsilon, int interval, int delta, float alpha)
    {
        N = num_procs;
        E = epsilon;
        I = interval;
        D = delta;
        A = alpha;
        time = 0;
        for(int i = 0; i < N; i++)
        {
            Process p = Process(i, epsilon, interval, N);
            vp.push_back(p);
        }
    }

    int GetTime()
    {
        return time;
    }

    void RandomUnicast();

};