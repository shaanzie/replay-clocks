#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "simulation.h"

using namespace std;

void Simulation::RandomUnicast(long long int absolute_time, string debugFile, string outFile)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> procgen(0, vp.size() - 1);

    std::uniform_int_distribution<int> alphagen(0, 100);
    std::uniform_int_distribution<int> deltagen(0, D);

    ofstream debug(debugFile);
    ofstream out(outFile);

    int num_events = 0;
    int totalOffsetSize = 0;
    int totalCounterSize = 0;

    for (long long sim_time = 0; sim_time <= absolute_time; sim_time++)
    {
        for (int proc = 0; proc < vp.size(); proc++)
        {
            vp[proc].GetMessages(vp[proc].GetPhyClock());
            
            vp[proc].ProcessMessages();

            if (alphagen(gen) <= A * 100)
            {
                int random_proc = procgen(gen);
                Message m = vp[proc].CreateMessage(sim_time + deltagen(gen), "");
                vp[random_proc].PushMsg(m);
                num_events ++;
            }

            vp[proc].Tick();
        }

        
        for(int proc = 0; proc < vp.size(); proc++)
        {
            totalOffsetSize += vp[proc].GetClock().OffsetSize();
            totalCounterSize += vp[proc].GetClock().CounterSize();
        }

    }
    cout << (float)totalOffsetSize / (vp.size()*num_events) << "," << (float)totalCounterSize / (vp.size()*num_events) << ",";
    cout << N << "," << E << "," << I << "," << D << "," << A << endl; 
}

int main(int argc, char* argv[])
{
    int num_procs = stoi(argv[1]);
    int epsilon = stoi(argv[2]);
    int interval = stoi(argv[3]);
    int delta = stoi(argv[4]);
    float alpha = stof(argv[5])/100;
    long long int absolute_time = stol(argv[6]);
    string debug = argv[7];
    string out = argv[8];

    Simulation s = Simulation(num_procs, epsilon, interval, delta, alpha);
    s.RandomUnicast(absolute_time, debug, out);

    return 0;
}