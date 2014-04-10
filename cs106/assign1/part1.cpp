#include <iostream>
#include <cmath>
#include "random.h"

using std::cout;
using std::endl;

bool IsPerfect(int n){
    int sum = 0;
    for(int i = 1; i <= static_cast<int>(sqrt(n)); i++){
        if(n % i == 0){
            sum += i + n / i;
        }
    }
    return (sum == 2*n);
}

int main(){
    for(int i = 2; i <= 10000; i++){
        if(IsPerfect(i)){
            cout << i << endl;
        }
    }
    return 0;
}
