/* File: boggle.cpp
 * ----------------
 * Your name here!
 */
 
#include "genlib.h"
#include "simpio.h"
#include <iostream>
#include "extgraph.h"
#include "gboggle.h"
#include <vector>
#include "random.h"
#include <algorithm>
#include "lexicon.h"
#include <set>
#include <cstdlib>

string StandardCubes[16]  = 
{"AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
 "DISTTY", "EEGHNW", "EEINSU", "EHRTVW", "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};
 
string BigBoggleCubes[25]  = 
{"AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM", "AEEGMU", "AEGMNN", "AFIRSY", 
"BJKQXZ", "CCNSTW", "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT", "DHHLOR", 
"DHLNOR", "EIIITT", "EMOTTT", "ENSSSU", "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};

int dir[8][2] = {
    {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
    {1, 0}, {1, -1}, {0, -1}, {-1, -1}
};

class Boggle {
    public:
        Boggle(string *board, int boardRow, int boardCol, int row, int col);
        Boggle(int row, int col);
        void HumanGame();
        void ComputerGame();
        ~Boggle();
    private:
        void _init(string *board, int boardRow, int boardCol, int row, int col);
        void shakeBoard(string *board, int boardRow, int boardCol);
        void renderBoard();
        bool findWord(int curRow, int curCol, int curIdx, string &word, set<int> &hasVisit);
        void getWord(int curRow, int curCol, string &prefix, set<int> &hasVisit);
        bool isValidCell(int r, int c);
        void highlightWord(set<int> &hasVisit);
        int row, col;
        string *board;
        Lexicon *lex;
        set<string> humanWords, computerWords;
};

Boggle::Boggle(string *board, int boardRow, int boardCol, int row, int col){
    _init(board, boardRow, boardCol, row, col);
}

Boggle::Boggle(int row, int col){
    _init(StandardCubes, 16, 6, row, col);
}

Boggle::~Boggle(){
    delete this->lex;
}

void Boggle::_init(string *board, int boardRow, int boardCol, int row, int col){
    this->row = row;
    this->col = col;
    this->board = board;
    this->lex = new Lexicon("lexicon.dat");
    shakeBoard(board, boardRow, boardCol);
    renderBoard();
}

void Boggle::shakeBoard(string *board, int boardRow, int boardCol){
    Randomize();
    for(int r = 0; r < boardRow; r++)
        for(int c = 0; c < boardCol; c++){
            int ranRow = RandomInteger(r, boardRow);
            int ranCol = RandomInteger(c, boardCol);
            char tmpChar = board[r][c];
            board[r][c] = board[ranRow][ranCol];
            board[ranRow][ranCol] = tmpChar;
        }
}

void Boggle::renderBoard(){
    DrawBoard(row, col);
    for(int r = 0; r < row; r++)
        for(int c = 0; c < col; c++){
            LabelCube(r, c, board[r][c]);
        }
}

bool Boggle::isValidCell(int r, int c){
    return r >= 0 && c >=0 && r < row && c < col;
}

void Boggle::highlightWord(set<int> &hasVisit){
    for(int x : hasVisit){
        int r = x / this->col;
        int c = x % this->col;
        HighlightCube(r, c, true);
    }

    sleep(2);

    for(int x : hasVisit){
        int r = x / this->col;
        int c = x % this->col;
        HighlightCube(r, c, false);
    }
}

bool Boggle::findWord(int curRow, int curCol, int nextIdx, string &word, set<int> &hasVisit){
    if(nextIdx == word.size()){
        return true;
    }
    for(int d = 0; d < 8; d++){
        int nextRow = curRow + dir[d][0];
        int nextCol = curCol + dir[d][1];
        if(isValidCell(nextRow, nextCol) && board[nextRow][nextCol] == word[nextIdx] &&
           hasVisit.find(nextRow*col+nextCol) == hasVisit.end() ){
            hasVisit.insert(nextRow*col+nextCol); 
            if(findWord(nextRow, nextCol, nextIdx+1, word, hasVisit)){
                return true;
            }else{
                hasVisit.erase(nextRow*col+nextCol); 
            }
        }
    }
    
    return false;
}

void Boggle::HumanGame(){
    string word;
    set<int> hasVisit; 
    while(getline(cin, word)){
        if(word == "")
            break;
        bool found = false;
        hasVisit.clear();
        for(int r = 0; r < row && !found; r++){
            for(int c = 0; c < col && !found; c++)
                if(board[r][c] == word[0]){
                    hasVisit.insert(r*col+c);
                    found = findWord(r, c, 1, word, hasVisit); 
                    if(found)
                        break;
                    hasVisit.clear();
                }
            }
        if(found && this->lex->containsWord(word) 
                 && humanWords.find(word) == humanWords.end()
                 && word.size() >= 4){
            highlightWord(hasVisit);
            RecordWordForPlayer(word, Human);
            humanWords.insert(word);
        }
    }
}

void Boggle::getWord(int curRow, int curCol, string &prefix, set<int> &hasVisit){
    if(!this->lex->containsPrefix(prefix)){
        return;
    }
    if(this->computerWords.find(prefix) == this->computerWords.end() &&
       this->humanWords.find(prefix) == this->humanWords.end() &&
       this->lex->containsWord(prefix) &&
       prefix.size() >= 4){
        this->computerWords.insert(prefix); 
        RecordWordForPlayer(prefix, Computer);
    }
    for(int d = 0; d < 8; d++){
        int nextRow = curRow + dir[d][0];
        int nextCol = curCol + dir[d][1];
        if(isValidCell(nextRow, nextCol) && hasVisit.find(nextRow*col+nextCol) == hasVisit.end()){
            prefix.push_back(board[nextRow][nextCol]);
            hasVisit.insert(nextRow*col+nextCol); 

            getWord(nextRow, nextCol, prefix, hasVisit);

            hasVisit.erase(nextRow*col+nextCol); 
            prefix.erase(prefix.end() - 1, prefix.end());
        }
    }
}

void Boggle::ComputerGame(){
    set<int> hasVisit;
    for(int r = 0; r < row; r++){
        for(int c = 0; c < col; c++){
            hasVisit.clear();
            hasVisit.insert(r*col+c);
            string prefix{board[r][c]};
            getWord(r, c, prefix, hasVisit); 
        }
    }
}

void GiveInstructions()
{
    cout << endl << "The boggle board is a grid onto which I will randomly distribute " 
	 << "cubes. These 6-sided cubes have letters rather than numbers on the faces, " 
	 << "creating a grid of letters on which you try to form words. You go first, " 
	 << "entering all the words you can find that are formed by tracing adjoining " 
	 << "letters. Two letters adjoin if they are next to each other horizontally, " 
	 << "vertically, or diagonally. A letter can only be used once in the word. Words "
	 << "must be at least 4 letters long and can only be counted once. You score points "
	 << "based on word length: a 4-letter word is worth 1 point, 5-letters earn 2 "
	 << "points, and so on. After your puny brain is exhausted, I, the super computer, "
	 << "will find all the remaining words and double or triple your paltry score." << endl;
	
    cout << "\nHit return when you're ready...";
    GetLine();
}

static void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle. " 
	 << "The good news is that you might improve your vocabulary a bit.  The "
	 << "bad news is that you're probably going to lose miserably to this little "
	 << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}


int main()
{
	InitGraphics();
	SetWindowSize(9, 5);
    Boggle b(BigBoggleCubes, 25, 6, 5, 5);
	Welcome();
	GiveInstructions();
    b.HumanGame();
    b.ComputerGame();
	return 0;
}
