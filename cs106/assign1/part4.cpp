#include <string>
#include <iostream>
#include <map>
#include <cctype>

using namespace std;

map<char, char> letterMap {{'A', '0'}, {'E', '0'}, {'I', '0'}, {'O', '0'}, {'U', '0'}, {'H', '0'}, {'W', '0'}, {'Y', '0'},
                           {'B', '1'}, {'F', '1'}, {'P', '1'}, {'V', '1'},
                           {'C', '2'}, {'G', '2'}, {'J', '2'}, {'K', '2'}, {'Q', '2'}, {'S', '2'}, {'X', '2'}, {'Z', '2'},
                           {'D', '3'}, {'T', '3'}, 
                           {'M', '4'}, {'N', '4'},
                           {'L', '5'},
                           {'R', '6'}};

int main(){
    string surname;
    for(;;){
        cout << "Enter surname (RETURN to quit):";
        if(!(cin >> surname)){
            break;
        }
        string code{}; 
        char preChar, newChar;
        for(int i = 0; i < surname.size(); i++){
            if(i == 0){
                code.push_back(toupper(surname[i]));
                preChar = code[0];
            }else{
                newChar = letterMap[toupper(surname[i])];
                if(newChar != preChar && newChar != '0'){
                    code.push_back(newChar);
                    preChar = newChar;
                }
            }
        }
        while(code.size() < 4)
            code.push_back('0');
        if(code.size() > 4)
            code = code.substr(0, 4);
        cout << "Soundex code for " << surname << " is " << code << endl;
    }
    return 0;
}
