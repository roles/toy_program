#include <iostream>
#include <vector>
#include <list>

using namespace std;

bool _CanMakeSum(vector<int>& nums, int targetSum, const vector<int>::iterator curIter){
    if(curIter == nums.end())
        return false;
    if(targetSum == *curIter)
        return true;
    return _CanMakeSum(nums, targetSum, curIter+1) || _CanMakeSum(nums, targetSum-*curIter, curIter+1);
}

bool CanMakeSum(vector<int>& nums, int targetSum){
    return _CanMakeSum(nums, targetSum, nums.begin()); 
}

void _ListAllSubset(vector<int>& nums, int targetSum, const vector<int>::iterator curIter, list<int>& numList){
    if(curIter == nums.end())
        return;
    if(*curIter == targetSum){
        cout << "{";
        for(auto n : numList){
           cout << n << ",";
        }
        cout << *curIter << "}" << endl;
        return;
    }
    _ListAllSubset(nums, targetSum, curIter+1, numList);

    numList.push_back(*curIter);
    _ListAllSubset(nums, targetSum-*curIter, curIter+1, numList);
    numList.pop_back();
}

void ListAllSubset(vector<int>& nums, int targetSum){
    list<int> numList;
    _ListAllSubset(nums, targetSum, nums.begin(), numList);
}

int main(){
    vector<int> nums{1, 2, 3, 4, 5}; 
    int targetSum;
    
    cout << "List number:";
    for(auto n : nums){
        cout << " " << n; 
    }
    cout << endl;

    cout << "Enter target sum: ";
    cin >> targetSum;

    if(CanMakeSum(nums, targetSum)){
        cout << "It can" << endl;
    }else{
        cout << "It cannot" << endl;
    }

    ListAllSubset(nums, targetSum);
}
