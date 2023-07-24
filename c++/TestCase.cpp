#include "REPCL.h"
#include <iostream>

using namespace std;

class TestCase
{
    REPCL e;
    REPCL f;
    REPCL m;
    REPCL f_true;
    int epsilon = 20;
    int interval = 10;
    int num_procs = 5;

public:
    TestCase()
    {
        e = REPCL(epsilon, interval, 0, 5);
        f = REPCL(epsilon, interval, 0, 5);
        m = REPCL(epsilon, interval, 3, 5);
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

    bool TestAdvanceScenario1()
    {
        int epoch = 40;
        vector<int> offsets = {5, 6, 7, 8, 9};
        vector<int> counters = {1, 2, 3, 4, 5};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        e.SendLocal(33);

        epoch = 40;
        offsets = {5, 6, 7, 8, 9};
        counters = {2, 2, 3, 4, 5};

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

    bool TestAdvanceScenario2()
    {
        int epoch = 40;
        vector<int> offsets = {5, 6, 7, 8, 9};
        vector<int> counters = {1, 2, 3, 4, 5};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        e.SendLocal(36);

        epoch = 40;
        offsets = {4, 6, 7, 8, 9};
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

    bool TestAdvanceScenario3()
    {
        int epoch = 40;
        vector<int> offsets = {5, 6, 7, 8, 9};
        vector<int> counters = {1, 2, 3, 4, 5};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        e.SendLocal(42);

        epoch = 42;
        offsets = {0, 8, 9, 10, 11};
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
        counters = {1, 0, 0, 0, 1};

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

    bool TestMergeScenario1()
    {
        int epoch = 40;
        vector<int> offsets = {5, 6, 7, 8, 9};
        vector<int> counters = {0, 1, 0, 0, 1};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        epoch = 45;
        offsets = {1, 2, 13, 4, 5};
        counters = {1, 0, 1, 0, 0};

        m.SetEpoch(epoch);
        m.SetOffsets(offsets);
        m.SetCounters(counters);

        e.Recv(m, 42);

        epoch = 45;
        offsets = {1, 2, 12, 4, 5};
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

    bool TestSize()
    {
        int epoch = 40;
        vector<int> offsets = {5, 6, 7, 8, 20};
        vector<int> counters = {0, 1, 0, 0, 1};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        if(e.OffsetSize() != 4 || e.CounterSize() != 2)
        {
            return false;
        }
        return true;
    }

    bool TestDeepCopy()
    {
        int epoch = 40;
        vector<int> offsets = {5, 6, 7, 8, 20};
        vector<int> counters = {0, 1, 0, 0, 1};

        e.SetEpoch(epoch);
        e.SetOffsets(offsets);
        e.SetCounters(counters);

        REPCL g = e;
        g.SetEpoch(49);

        if(e.IsEqual(g))
        {
            return false;
        }
        return true;
    }

};

int main()
{

    TestCase t;
    cout << "Test result for Advance in same epoch: " << t.TestAdvanceSameEpoch() << endl;
    cout << "Test result for Advance in diff epoch: " << t.TestAdvanceDiffEpoch() << endl;
    cout << "Test result for Advance in diff epoch which is epsilon away: " << t.TestAdvanceDiffEpochEps() << endl;
    cout << "Test result for Advance Scenario 1: " << t.TestAdvanceScenario1() << endl;
    cout << "Test result for Advance Scenario 2: " << t.TestAdvanceScenario2() << endl;
    cout << "Test result for Advance Scenario 3: " << t.TestAdvanceScenario3() << endl;
    cout << "Test result for Merge in same epoch: " << t.TestMergeSameEpoch() << endl;
    cout << "Test result for Merge in lagging epoch: " << t.TestMergeLagging() << endl;
    cout << "Test result for Merge in lagging epoch by eps: " << t.TestMergeLaggingEps() << endl;
    cout << "Test result for Merge in leading epoch: " << t.TestMergeLeading() << endl;
    cout << "Test result for Merge in Advanced epoch: " << t.TestMergeAdvance() << endl;
    cout << "Test result for Merge Scenario 1: " << t.TestMergeScenario1() << endl;
    cout << "Test result for sizes: " << t.TestSize() << endl;
    cout << "Test result for deepcopy: " << t.TestDeepCopy() << endl;
    return 0;
}