#include "random.h"
#include "simpio.h"
#include <iostream>

using namespace std;

void readInput(int& numVoter, double& diffPer, double& errPer){
    cout << "Enter number of voters: ";
    numVoter = GetInteger();
    cout << "Enter percentage spread between candidates: ";
    diffPer = GetReal();
    cout << "Enter voting error percentage: ";
    errPer = GetReal();
}

void getCanNum(const int& numVoter, const double& diffPer, 
               int& numCanA, int& numCanB){
    //numCanA = (1 + diffPer) / 2 * numVoter;
    //numCanB = numVoter - numCanA;
    numCanA = (0.5 + diffPer) * numVoter;
    numCanB = numVoter - numCanA;
}

int getRealVote(const int& voteCount, const double& errPer,
                int& preciseVote, int& errorVote){
    for(int i = 0; i < voteCount; i++){ 
        if(!RandomChance(errPer)){
            preciseVote++;
        }else{
            errorVote++;
        }
    }
}

int main(){
    int numVoter;
    double diffPer, errPer;
    int numCanA, numCanB;
    int numVoteA, numVoteB;
    const int numTrail = 500;
    int invalidTrail = 0;

    Randomize(); 
    readInput(numVoter, diffPer, errPer);
    getCanNum(numVoter, diffPer, numCanA, numCanB);
    for(int i = 0; i < numTrail; i++){
        numVoteA = numVoteB = 0;
        getRealVote(numCanA, errPer, numVoteA, numVoteB);
        getRealVote(numCanB, errPer, numVoteB, numVoteA);
        if(numVoteB >= numVoteA){
            ++invalidTrail; 
        }
    }

    cout << "Chance of an invalid election result after 500 trails = "
         << invalidTrail * 100.0 / numTrail << "%" << endl;

    return 0;
}
