#include "rc.h"
#include <iostream>

using namespace std;


class TestCaseRC
{

    RC e;
    RC f;
    RC m;

    public:

    TestCaseRC()
    {
        e = RC(0, 5);
        f = RC(0, 5);
        m = RC(1, 5);
    }

    bool TestAdvanceSameEpoch()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        e.SetOffsets(0b0010100000000000000100000);
        e.SetCounters(2);

        f = e;

        // Only counters increase
        e.SendLocal(40);

        f.SetCounters(3);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestAdvanceDiffEpoch()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 00101 00000 00000 00001 00000
        e.SetOffsets(0b0010100000000000000100000);
        e.SetCounters(1);
        f = e;

        // Offsets change
        e.SendLocal(45);

        f.SetHLC(45);
        // 0b 01010 00101 00101 00110 00000
        f.SetOffsets(0b0101000101001010011000000);
        f.SetCounters(0);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestAdvanceDiffEpochEps()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 00101 00000 00000 00001 00000
        // 0, 1, 0, 0, 5 - in reverse
        e.SetOffsets(0b0010100000000000000100000);
        e.SetCounters(1);
        f = e;

        // Offsets change
        e.SendLocal(56);

        f.SetHLC(56);
        f.SetOffsetBitmap(0b01111);
        // 0b 00000 10000 10000 10001 00000
        // 0, 17, 16, 16, e
        f.SetOffsets(0b0000010000100001000100000);
        f.SetCounters(0);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestAdvanceScenario1()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 01001 01000 00111 00110 00101
        // 5, 6, 7, 8, 9 - in reverse
        e.SetOffsets(0b0100101000001110011000101);
        e.SetCounters(15);
        f = e;

        e.SendLocal(33);

        f.SetHLC(40);
        f.SetOffsetBitmap(0b11111);
        // 0b 01001 01000 00111 00110 00101
        // 5, 6, 7, 8, 9 - in reverse
        f.SetOffsets(0b0100101000001110011000101);
        f.SetCounters(16);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestAdvanceScenario2()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 01001 01000 00111 00110 00101
        // 5, 6, 7, 8, 9 - in reverse
        e.SetOffsets(0b0100101000001110011000101);
        e.SetCounters(15);
        f = e;

        e.SendLocal(36);

        f.SetHLC(40);
        f.SetOffsetBitmap(0b11111);
        // 0b 01001 01000 00111 00110 00100
        // 4, 6, 7, 8, 9 - in reverse
        f.SetOffsets(0b0100101000001110011000100);
        f.SetCounters(0);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestAdvanceScenario3()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 01001 01000 00111 00110 00101
        // 5, 6, 7, 8, 9 - in reverse
        e.SetOffsets(0b0100101000001110011000101);
        e.SetCounters(15);
        f = e;

        e.SendLocal(42);

        f.SetHLC(42);
        f.SetOffsetBitmap(0b11111);
        // 0b 01011 01010 01001 01000 00000
        // 0, 8, 9, 10, 11 - in reverse
        f.SetOffsets(0b0101101010010010100000000);
        f.SetCounters(0);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestMergeSameEpoch()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        e.SetOffsets(0b0010100000000000000100000);
        e.SetCounters(2);

        f = e;

        m.SetHLC(40);
        m.SetOffsetBitmap(0b11111);
        // 0b 00010 00001 00111 00000 00101
        // 5, 0, 7, 1, 2 in reverse
        m.SetOffsets(0b0001000001001110000000101);
        m.SetCounters(0);

        // Only counters increase
        e.Recv(m, 40);

        f.SetOffsets(0b0001000000000000000000000);
        f.SetCounters(0);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestMergeLagging()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 01000 00000 00000 00001 00000
        // 0, 1, 0, 0, 8 in reverse
        e.SetOffsets(0b0100000000000000000100000);
        e.SetCounters(1);

        f = e;

        m.SetHLC(35);
        m.SetOffsetBitmap(0b11111);
        // 0b 00001 00000 00010 00000 00001
        // 1, 0, 2, 0, 1 in reverse
        m.SetOffsets(0b0000100000000100000000001);
        m.SetCounters(2);

        // Only counters increase
        e.Recv(m, 40);
        
        // 0b 00110 00000 00000 00001 00000
        // 0, 1, 0, 0, 6 in reverse
        f.SetOffsets(0b0011000000000000000100000);
        f.SetCounters(0);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestMergeLaggingEps()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 00001 00000 00000 00001 00000
        // 0, 1, 0, 0, 1 in reverse
        e.SetOffsets(0b0000100000000000000100000);
        e.SetCounters(1);

        f = e;

        m.SetHLC(35);
        m.SetOffsetBitmap(0b11111);
        // 0b 00001 00000 00010 00000 00001
        // 1, 0, 2, 0, 1 in reverse
        m.SetOffsets(0b0000100000000100000000001);
        m.SetCounters(2);

        // Only counters increase
        e.Recv(m, 40);
        
        // 0b 00001 00000 00000 00001 00000
        // 0, 1, 0, 0, 1 in reverse
        f.SetOffsets(0b0000100000000000000100000);
        f.SetCounters(2);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestMergeLeading()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 00111 00000 00000 00001 00000
        // 0, 1, 0, 0, 7 in reverse
        e.SetOffsets(0b0011100000000000000100000);
        e.SetCounters(1);

        f = e;

        m.SetHLC(45);
        m.SetOffsetBitmap(0b11111);
        // 0b 00000 00010 00001 00000 00111
        // 7, 0, 1, 2, 0 in reverse
        m.SetOffsets(0b0000000010000010000000111);
        m.SetCounters(2);

        // Only counters increase
        e.Recv(m, 40);
        
        f.SetHLC(45);
        // 0b 00000 00010 00001 00000 00101
        // 5, 0, 1, 2, 0 in reverse
        f.SetOffsets(0b0000000010000010000000101);
        f.SetCounters(0);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestMergeAdvance()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 00111 00000 00000 00001 00000
        // 0, 1, 0, 0, 7 in reverse
        e.SetOffsets(0b0011100000000000000100000);
        e.SetCounters(1);

        f = e;

        m.SetHLC(40);
        m.SetOffsetBitmap(0b11111);
        // 0b 00000 00010 00001 00000 00111
        // 7, 0, 1, 2, 0 in reverse
        m.SetOffsets(0b0000000010000010000000111);
        m.SetCounters(2);

        // Only counters increase
        e.Recv(m, 45);
        
        f.SetHLC(45);
        // 0b 00101 00101 00101 00101 00101
        // 5, 5, 5, 5, 5 in reverse
        f.SetOffsets(0b0010100101001010010100101);
        f.SetCounters(0);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

    bool TestMergeScenario1()
    {
        e.SetHLC(40);
        e.SetOffsetBitmap(0b11111);
        // 0b 01001 01000 00111 00110 00101
        // 5, 6, 7, 8, 9 - in reverse
        e.SetOffsets(0b0100101000001110011000101);
        e.SetCounters(15);
        f = e;

        
        m.SetHLC(45);
        // 0b 00101 00100 01101 00010 00001
        // 1, 2, 13, 4, 5 in reverse
        m.SetOffsets(0b0010100100011010001000001);
        m.SetCounters(3);

        e.Recv(m, 42);

        f.SetHLC(45);
        f.SetOffsetBitmap(0b11111);
        // 0b 00101 00100 01100 00010 00001
        // 1, 2, 12, 4, 5 - in reverse
        f.SetOffsets(0b0010100100011000001000001);
        f.SetCounters(0);

        bool equal = e.IsEqual(f);
        if (!equal)
        {
            cout << endl
                    << "Equality failed!";
            cout << endl
                 << "e: " << endl;
            e.PrintClock();
            cout << endl
                 << "f: " << endl;
            f.PrintClock();
            cout << endl;
        }
        return equal;
    }

};

int main()
{

    TestCaseRC t;
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
    return 0;
}