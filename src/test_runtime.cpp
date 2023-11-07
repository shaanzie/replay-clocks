#include<iostream>
#include<bitset>
#include<math.h>

using namespace std;

void one_pos(int num){
    int n = num;
    while(n>0){
        int pos = log2((~(n ^ (~(n - 1))) + 1) >> 1);
        std::cout << pos << std::endl;
        n = n & (n-1);
    }
}

int extract(int number, int k, int p)
{
    return (((1 << k) - 1) & (number >> (p)));
}

void insert(bitset<64>& offsets, bitset<4> new_offset, int index)
{

    bitset<64> res(extract(offsets.to_ulong(), index, 0));
    
    res |= new_offset.to_ulong() << index;

    bitset<64> lastpart(extract(offsets.to_ulong(), 64 - index - 4, index + 1));
    
    res |= (lastpart << (index + 4));

    offsets = res;

}

void remove(bitset<64>& offsets, int index)
{
    
    bitset<64> res(extract(offsets.to_ulong(), index, 0));
    bitset<64> lastpart(extract(offsets.to_ulong(), 64 - index - 4, index + 4));

    res |= (lastpart << index);

    offsets = res;


}

int getoffset(bitset<64>& offsets, int index)
{
    
    bitset<64> offset(extract(offsets.to_ulong(), 4, 4*index));

    return offset.to_ulong();

}

int main()
{
    // int num = 0;
    // num = num | 0b1111;
    // cout << num << endl;
    // one_pos(num);

    bitset<64> offsets(0b00110101);
    bitset<4> offset(0b1111);
    int index = 0;
    
    cout << offsets << endl;

    cout << getoffset(offsets, 1) << endl;

    insert(offsets, offset, index);
    cout << "After insert" << endl;
    cout << offsets << endl;

    remove(offsets, 4);
    cout << "After remove" << endl;
    cout << offsets << endl;


}

