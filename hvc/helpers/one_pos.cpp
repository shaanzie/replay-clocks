#include<iostream>
void one_pos(int);
int main()
{
    int num = 0b1101000100;
    one_pos(num);
}

void one_pos(int num){
    int n = num;
    while(n>0){
        int pos = (~(n ^ (~(n - 1))) + 1) >> 1;
        std::cout << pos << std::endl;
        n = n & (n-1);
    }
}
