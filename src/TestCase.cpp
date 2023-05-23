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

    bool TestMergeSameEpoch()
    {
        int epoch = 40;
        vector<int> offsets = {0, 1, 0, 0, 5};
        vector<int> counters = {0, 0, 0, 1, 0};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        epoch = 40;
        offsets = {5, 0, 7, 1, 2};
        counters = {0, 0, 1, 0, 1};

        m.SetEpoch(epoch);
        m.SetOffsets(offsets);
        m.SetCounters(counters);

        e.Recv(m, 40);

        epoch = 40;
        offsets = {0, 0, 0, 0, 2};
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

    bool TestMergeLagging()
    {
        int epoch = 40;
        vector<int> offsets = {0, 1, 0, 0, 8};
        vector<int> counters = {0, 0, 0, 0, 1};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        epoch = 35;
        offsets = {1, 0, 2, 0, 1};
        counters = {0, 0, 1, 0, 1};

        m.SetEpoch(epoch);
        m.SetOffsets(offsets);
        m.SetCounters(counters);

        e.Recv(m, 40);

        epoch = 40;
        offsets = {0, 1, 0, 0, 6};
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

    bool TestMergeLaggingEps()
    {
        int epoch = 40;
        vector<int> offsets = {0, 1, 0, 0, 1};
        vector<int> counters = {0, 0, 0, 0, 1};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        epoch = 35;
        offsets = {1, 0, 2, 0, 1};
        counters = {0, 0, 1, 0, 1};

        m.SetEpoch(epoch);
        m.SetOffsets(offsets);
        m.SetCounters(counters);

        e.Recv(m, 40);

        epoch = 40;
        offsets = {0, 1, 0, 0, 1};
        counters = {1, 0, 0, 0, 0};

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

    bool TestMergeLeading()
    {
        int epoch = 40;
        vector<int> offsets = {0, 1, 0, 0, 7};
        vector<int> counters = {0, 0, 0, 1, 0};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        epoch = 45;
        offsets = {7, 0, 1, 2, 0};
        counters = {1, 0, 1, 0, 0};

        m.SetEpoch(epoch);
        m.SetOffsets(offsets);
        m.SetCounters(counters);

        e.Recv(m, 40);

        epoch = 45;
        offsets = {5, 0, 1, 2, 0};
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

    bool TestMergeAdvance()
    {
        int epoch = 40;
        vector<int> offsets = {0, 1, 0, 0, 7};
        vector<int> counters = {0, 0, 0, 1, 0};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        epoch = 40;
        offsets = {7, 0, 1, 2, 0};
        counters = {1, 0, 1, 0, 0};

        m.SetEpoch(epoch);
        m.SetOffsets(offsets);
        m.SetCounters(counters);

        e.Recv(m, 45);

        epoch = 45;
        offsets = {5, 5, 5, 5, 5};
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
    cout << "Test result for Merge in same epoch: " << t.TestMergeSameEpoch() << endl;
    cout << "Test result for Merge in lagging epoch: " << t.TestMergeLagging() << endl;
    cout << "Test result for Merge in lagging epoch by eps: " << t.TestMergeLaggingEps() << endl;
    cout << "Test result for Merge in leading epoch: " << t.TestMergeLeading() << endl;
    cout << "Test result for Merge in Advanced epoch: " << t.TestMergeAdvance() << endl;
    return 0;
}