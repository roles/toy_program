#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void getHistogram(ifstream& ifs, vector<int>& htgram){
    int score;
    while(ifs >> score){
        ++htgram[score/10];
    }
}

void printHistogram(vector<int>& htgram){
    for(int i = 0; i < 10; i++){
        cout << i * 10 << "-" << (i+1) * 10 - 1 << ": ";
        for(int j = 0; j < htgram[i]; j++)
            cout << 'X';
        cout << endl;
    }
}

int main(){
    ifstream ifs;
    vector<int> htgram(10);
    for(;;){
        string scoreFile;
        cout << "Enter score file : ";
        cin >> scoreFile;
        ifs.open(scoreFile, ifstream::in);
        if((ifs.rdstate() & ifstream::failbit) == 0)        //检测是否成功打开文件
            break;
        cout << "cannot open file " << scoreFile << endl; 
    }

    getHistogram(ifs, htgram);
    printHistogram(htgram);

    ifs.close();
    return 0;
}
