#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include "random.h"

using namespace std;

class TextAnalyser{
    private:
        string curSeed;
        map<string, vector<char> > occur;
        int order;

    public:
        TextAnalyser(ifstream& ifs, int order);
        string randomText();
        string getMaxOccur();
};

ifstream& consoleOpenFile(ifstream& ifs){
    for(;;){
        string scoreFile;
        cout << "Enter score file : ";
        cin >> scoreFile;
        ifs.open(scoreFile, ifstream::in);
        if((ifs.rdstate() & ifstream::failbit) == 0)        //检测是否成功打开文件
            break;
        cout << "cannot open file " << scoreFile << endl; 
    }
    return ifs;
}

TextAnalyser::TextAnalyser(ifstream& ifs, int order) 
    : order{order}
{
    list<char> clist{};
    char c;
    string seed;

    while(clist.size() < order){
        ifs.get(c);
        clist.push_back(c);
    }
    while(!ifs.eof()){
        ifs.get(c);
        seed = string(clist.begin(), clist.end());
        this->occur[seed].push_back(c);
        clist.push_back(c);
        clist.pop_front();
    }
}

string TextAnalyser::getMaxOccur(){
    string maxOccurStr;
    int maxOccur = 0;
    for(auto o : this->occur){
        if(o.second.size() > maxOccur){
            maxOccurStr = o.first;
            maxOccur = o.second.size();
        }
    }
    return maxOccurStr;
}

string TextAnalyser::randomText(){
    string seed = this->getMaxOccur();
    string text{seed}; 
    list<char> clist{};
    
    for(char c : seed){
        clist.push_back(c);
    }

    while(text.size() < 2000){
        if(this->occur.find(seed) == this->occur.end()){
            return text;
        }
        vector<char>& charCollect = this->occur[seed]; 
        int i = RandomInteger(0, charCollect.size()); 
        text.push_back(charCollect[i]);

        clist.push_back(charCollect[i]);
        clist.pop_front();
        seed.assign(clist.begin(), clist.end());
    }

    return text;
}

int main(){
    int order;
    ifstream ifs;

    Randomize();
    consoleOpenFile(ifs);
    cout << "Enter order: ";
    cin >> order;

    TextAnalyser ta{ifs, order};

    cout << ta.randomText() << endl;

    ifs.close(); 
    return 0;
}
