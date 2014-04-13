#include <iostream>
#include "simpio.h"
#include "graphics.h"
#include "extgraph.h"
#include <vector>
#include <cmath>
#include <string>
#include "lexicon.h"
#include <map>

using namespace std;

int CountWays(int numStairs){
    if(0 == numStairs){
        return 1;
    }else if(1 == numStairs){
        return 1;
    }
    return CountWays(numStairs-1) + CountWays(numStairs-2);
}

void testCountWays(){
    cout << "Enter number of stairs: ";
    int numStairs = GetInteger();
    cout << "Number of ways: " << CountWays(numStairs) << endl;
}

void _DrawRuler(double x, double y, double w, double h){
    const static double hlim = 0.01;
    if(h < hlim)
        return;
    MovePen(x + w/2, y);
    DrawLine(0, h);
    UpdateDisplay();
    _DrawRuler(x, y, w/2, h/2);
    _DrawRuler(x + w/2, y, w/2, h/2);
}

void DrawRuler(double x, double y, double w, double h){
    InitGraphics();
    MovePen(x, y);
    DrawLine(w, 0);
    UpdateDisplay();

    _DrawRuler(x, y, w, h);
}

void testDrawRuler(){
    DrawRuler(0.5, 0.5, 2, 0.5);
}

/*
 * Function: _CountCriticalVotes
 * --------------------
 * 假设除了blocksIndex之外block的和为N，整个投票分为A和N-A，blocksIndex对应的投票为X
 * 若假定A少于一半，N/2 >= A
 * 要使A最终能反超，要A >= (N-X)/2 
 * 递归的枚举A，看有多少中情况A在这个范围内
 */
int _CountCriticalVotes(vector<int>& blocks, int blocksIndex, int curIndex,
                        int votes, int lrange, int rrange){
    if(curIndex == blocks.size()){
        if((votes >= lrange) && (votes <= rrange))
            return 1;
        else
            return 0;
    }
    if(curIndex == blocksIndex)
        return _CountCriticalVotes(blocks, blocksIndex, curIndex+1, votes, lrange, rrange);
    return ((votes < rrange) ? _CountCriticalVotes(blocks, blocksIndex, curIndex+1, votes+blocks[curIndex], lrange, rrange) : 0) +
           _CountCriticalVotes(blocks, blocksIndex, curIndex+1, votes, lrange, rrange);
}

int CountCriticalVotes(vector<int>& blocks, int blocksIndex){
    int sum = 0;
    for(auto b : blocks){
        sum += b;
    }
    sum -= blocks[blocksIndex];
    int lrange = ceil((sum - blocks[blocksIndex]) / 2);
    int rrange = floor(sum / 2);

    _CountCriticalVotes(blocks, blocksIndex, 0, 0, lrange, rrange);
}

void testCountCriticalVotes(){
    vector<int> blocks{9, 9, 7, 3, 1, 1};
    int blocksIndex;

    cout << "Blocks: ";
    for(auto b : blocks){
        cout << b << " ";
    }
    cout << endl;
    cout << "Enter block index: ";
    cin >> blocksIndex;

    cout << "Number of Critical Votes: " << CountCriticalVotes(blocks, blocksIndex) << endl;
}

void _PrintPrefixWords(string words, string& prefix){
    for(auto pprefix = prefix.begin(), pwords = words.begin(); pprefix != prefix.end(); pprefix++, pwords++){
        if(pwords == words.end() || (*pwords != *pprefix))
            return;
    }
    cout << words << endl;
}

void _ListCompletions(string& digitSequence, Lexicon& lex, 
                      int curDigitIndex, string& curPrefix){
    static map<char, string> keyMap{{'2', string{"abc"}}, {'3', string{"def"}}, 
                                    {'4', string{"ghi"}}, {'5', string{"jkl"}},
                                    {'6', string{"mno"}}, {'7', string{"pqrs"}},
                                    {'8', string{"tuv"}}, {'9', string{"wxyz"}}
    };    

    if(!lex.containsPrefix(curPrefix)){
        return;
    }
    if(curDigitIndex == digitSequence.size()){
        lex.mapAll(_PrintPrefixWords,  curPrefix);
        return;
    }
    for(char c : keyMap[digitSequence[curDigitIndex]]){
        curPrefix.push_back(c);
        _ListCompletions(digitSequence, lex, curDigitIndex+1, curPrefix);
        curPrefix.erase(curPrefix.size()-1);
    }
}

void ListCompletions(string digitSequence, Lexicon& lex){
    string prefix{};
    _ListCompletions(digitSequence, lex, 0, prefix);
}

void testListCompletions(){
    Lexicon lex{"lexicon.dat"};
    string digitSequence;
    cout << "Enter digit sequence: ";
    cin >> digitSequence;

    ListCompletions(digitSequence, lex);
}

bool _SolvePuzzle(vector<int>& squares, int curIndex, set<int>& hasVisit){
    if(curIndex < 0 || curIndex >= squares.size() || hasVisit.find(curIndex) != hasVisit.end()){
        return false;
    }
    if(squares[curIndex] == 0){
        return true;
    }
    hasVisit.insert(curIndex); 
    return _SolvePuzzle(squares, curIndex + squares[curIndex], hasVisit) ||
           _SolvePuzzle(squares, curIndex - squares[curIndex], hasVisit);
}

bool Solvable(int start, vector<int>& squares){
    set<int> hasVisit{};
    return _SolvePuzzle(squares, start, hasVisit);
}

void testSolvable(){
    //vector<int> squares{3, 6, 4, 1, 3, 4, 2, 5, 3, 0};
    vector<int> squares{3, 1, 4, 1, 0};
    int num;

    cout << "Enter start index: ";
    num = GetInteger();
    if(Solvable(num, squares)){
        cout << "It can" << endl;
    }else{
        cout << "It cannot" << endl;
    }
}



int main(){
    //testCountWays();
    //testDrawRuler();
    //testCountCriticalVotes(); 
    //testListCompletions();
    testSolvable();

    return 0;
}
