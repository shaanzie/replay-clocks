#include "hvc.h"
#include <iostream>

using namespace std;

class TestCase
{
    HVC e;
    HVC f;
    HVC m;
    HVC f_true;
    int epsilon = 20;
    int interval = 10;
    int num_procs = 5;

public:
    TestCase()
    {
        e = HVC(epsilon, interval, 0, 5);
        f = HVC(epsilon, interval, 0, 5);
        m = HVC(epsilon, interval, 3, 5);
    }

    bool TestAdvanceSameEpoch()
    {
        int epoch = 40;
        vector<int> offsets = {0, 1, 0, 0, 5};
        vector<int> counters = {0, 0, 0, 1, 0};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        e.SendLocal(40);

        epoch = 40;
        offsets = {0, 1, 0, 0, 5};
        counters = {1, 0, 0, 1, 0};

        f.SetEpoch(epoch);
        f.SetOffsets(offsets);
        f.SetCounters(counters);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.printClock();
            cout << endl
                 << "f: " << endl;
            f.printClock();
            cout << endl;
        }
        return equal;
    }

    bool TestAdvanceDiffEpoch()
    {
        int epoch = 40;
        vector<int> offsets = {0, 1, 0, 0, 5};
        vector<int> counters = {0, 0, 0, 1, 0};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        e.SendLocal(45);

        epoch = 45;
        offsets = {0, 6, 5, 5, 10};
        counters = {0, 0, 0, 0, 0};

        f.SetEpoch(epoch);
        f.SetOffsets(offsets);
        f.SetCounters(counters);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.printClock();
            cout << endl
                 << "f: " << endl;
            f.printClock();
            cout << endl;
        }
        return equal;
    }

    bool TestAdvanceDiffEpochEps()
    {
        int epoch = 40;
        vector<int> offsets = {0, 1, 0, 0, 5};
        vector<int> counters = {0, 0, 0, 1, 0};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        e.SendLocal(56);

        epoch = 56;
        offsets = {0, 17, 16, 16, 20};
        counters = {0, 0, 0, 0, 0};

        f.SetEpoch(epoch);
        f.SetOffsets(offsets);
        f.SetCounters(counters);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.printClock();
            cout << endl
                 << "f: " << endl;
            f.printClock();
            cout << endl;
        }
        return equal;
    }


};

int main()
{

    TestCase t;
    cout << "Test result for Advance in same epoch: " << t.TestAdvanceSameEpoch() << endl;
    cout << "Test result for Advance in diff epoch: " << t.TestAdvanceDiffEpoch() << endl;
    cout << "Test result for Advance in diff epoch which is epsilon away: " << t.TestAdvanceDiffEpochEps() << endl;
    return 0;
}