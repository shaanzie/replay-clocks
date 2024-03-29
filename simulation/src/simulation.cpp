#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <climits>
#include "simulation.h"

using namespace std;

int GetMinEpoch(vector<Process> &vp)
{
    int min_epoch = INT_MAX;
    for (int proc = 0; proc < vp.size(); proc++)
    {
        min_epoch = min(vp[proc].GetClock().GetHLC(), min_epoch);
    }
    return min_epoch;
}

void Simulation::RandomUnicast()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> procgen(0, vp.size() - 1);

    std::uniform_real_distribution<float> alphagen(0.0, 100.0);
    std::uniform_int_distribution<int> advancegen(0, 100);

    long long int num_events = 0;
    long long int totalOffsetSize = 0;
    long long int totalCounterSize = 0;        
    int maxOffset = 0;
    int maxCounter = 0;
    double avg_time_for_send = 0;
    double avg_time_for_recv = 0;

    // Check sorrachai's paper for similar results
    // Collect information on: if we don't store epoch, what is the error rate in causality
    // How many events had all counters to be 0

    for (int sim_time = E * I; sim_time <= E * I + 100000; sim_time++)
    {

        for (int proc = 0; proc < vp.size(); proc++)
        {
            avg_time_for_recv += vp[proc].ProcessMessages();

            // if(proc == 0)
            // {
            //     cout << vp[proc].GetClock() << endl;
            // }

            if (vp[proc].GetClock().GetHLC() - GetMinEpoch(vp) >= E)
            {
                continue;
            }
            
            float rand = alphagen(gen);

            if (rand < A * 100)
            { 

                int random_proc = procgen(gen);
                while (random_proc == proc)
                {
                    random_proc = procgen(gen);
                }

                auto start = chrono::high_resolution_clock::now();

                vp[proc].Send();

                auto stop = chrono::high_resolution_clock::now();
                auto send_duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);

                avg_time_for_send += send_duration.count();
                

                Message m = vp[proc].CreateMessage(vp[random_proc].GetPhyClock() + (D), "");
                vp[random_proc].PushMsg(m);

                num_events++;
            }

            // if (advancegen(gen) < 50)
            // {
            vp[proc].Tick();
            // }

            // this_thread::sleep_for(std::chrono::seconds(1));
        }

        int offsize = 0;

        for (int proc = 0; proc < vp.size(); proc++)
        {
            totalOffsetSize += vp[proc].GetClock().OffsetSize();
            totalCounterSize += vp[proc].GetClock().CounterSize();

            offsize += vp[proc].GetClock().OffsetSize();

            // vector<int> offs = vp[proc].GetClock().GetOffsets();
            // vector<int> counts = vp[proc].GetClock().GetCounters();
            
            // int maxoff = 0;
            // for(int i = 0; i < offs.size(); i++)
            // {
            //     if(offs[i] != E)
            //     {
            //         maxoff = max(maxoff, offs[i]);
            //     }
            // }
            // maxOffset = max(maxoff, maxOffset);

            // int maxcount = *max_element(counts.begin(), counts.end());

            // maxOffset = max(maxOffset, maxoff);
            // maxCounter = max(maxcount, maxCounter);

            // cout << "Process " << proc << endl << vp[proc].GetClock() << "Physical Clock: " << vp[proc].GetPhyClock() << endl << "Offsize: " << vp[proc].GetClock().OffsetSize() << endl << endl;
            // this_thread::sleep_for(std::chrono::seconds(1));
        }
        // cout << offsize << " ";
        // if(offsize == 0)
        // {
        //     exit(0);
        // }
    }

    // Measure system time get

    auto start = chrono::high_resolution_clock::now();

    auto test = chrono::high_resolution_clock::now();

    auto stop = chrono::high_resolution_clock::now();
    auto test_duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);

    num_events++;
    cout << (float)totalOffsetSize / (100000 * vp.size()) << "," << (float)totalCounterSize / (100000 * vp.size()) << ",";
    cout << N << "," << E << "," << I << "," << D << "," << A << "," << num_events << "," ;
    cout << avg_time_for_send / num_events << "," << avg_time_for_recv / num_events << "," << test_duration.count() << endl;
}

int main(int argc, char *argv[])
{
    int num_procs = stoi(argv[1]);
    int epsilon = stoi(argv[2]);
    int interval = stoi(argv[3]);
    int delta = stoi(argv[4]);
    float alpha = stof(argv[5]) / 10000;

    Simulation s = Simulation(num_procs, epsilon, interval, delta, alpha);
    s.RandomUnicast();

    return 0;
}