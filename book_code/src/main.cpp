#include "dc.h"

using namespace std;
using namespace Lexer;
using Parser::expr;
using Table::table;
using namespace Error;

void Dirver::calculate(){
    while(true){
        ts.get();
        if(ts.current().kind == Kind::end)
            break;
        if(ts.current().kind == Kind::print)
            continue;
        cout << expr(false) << endl;
        //cout << ts.current();
    }
}

int main(int argc, char* argv[]){

    table["pi"] = 3.1415926;
    table["e"] = 2.7182818;

    switch(argc){
        case 1:
            break;
        case 2:
            ts.set_input(new istringstream{argv[1]});
        default:
            error("too many arguments");
            return 1;
    }

    Dirver::calculate();
    return no_of_errors;
}
