#include "simpio.h"
#include <iostream>

using namespace std;

/*
 * Function : PrintInBinary
 * Usage : PrintInBinary(num);
 * -------------------
 * 给定一个整数，打印其二进制形式
 * 递归的原理是每一次对2求余可以知道是奇数还是偶数，
 * 从而确定最后一位二进制位是多少
 */
void PrintInBinary(int num){
    if(num != 0){
        PrintInBinary(num / 2);
        cout << num % 2;
    }
}

int main(){
    int num;
    cout << "Enter a number : ";
    num = GetInteger(); 
    cout << "Binary format : ";
    PrintInBinary(num);
    cout << endl;
    return 0;
}
