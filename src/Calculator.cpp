#include"Calculator.h"

int no_of_errors;
Token_stream ts{cin};
map<string, double> table;

ostream& operator<<(ostream &os, Token& t){
    switch(t.kind){
        case Kind::number:
            os << "Number Token : " << t.number_value << endl;
            break;
        case Kind::name:
            os << "Name Token : " << t.string_value << endl;
            break;
        case Kind::end:
            os << "End token" << endl;
            break;
        default:
            os << "Operator token : " << static_cast<char>(t.kind) << endl;
            break;
    }
    return os;
}

double error(const string &s){
    no_of_errors++;
    cerr << "error: " << s << endl;
    return 1;
}

Token Token_stream::get(){
    char ch = 0;

    //*ip >> ch;                        //这种读入方式直到空白字符才结束，
                                        //会导致x=7这种读入处理有误

    do{                                 //这种读入方式可以获取下一个非空白字符
        if(!ip->get(ch))
            return ct = {Kind::end};
    }while(ch != '\n' && isspace(ch));

    switch(ch){
        case 0:                         //ch为0标志读入的结束
            return ct = {Kind::end};
        case ';': case '\n':
            return ct = {Kind::print};
        case '*': case '/': case '+':
        case '-': case '(': case ')':
        case '=':                       //利用枚举类型的基本为char类型进行转换
            return ct = {static_cast<Kind>(ch)};
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': 
        case '.':                       //处理数字的时候将第一个字符写回到istream中
            ip->putback(ch);
            *ip >> ct.number_value;
            ct.kind = Kind::number;
            return ct;
        default:                        //要处理Name, Name=，或者error三种情况
            if(isalpha(ch)){
                ct.string_value = ch;
                while(ip->get(ch) && isalnum(ch))
                    ct.string_value += ch;
                ip->putback(ch);
                ct.kind = Kind::name;
                return ct;
            }

            //error("bad token");
            return ct = {Kind::print};
    }
}

Token& Token_stream::current(){
    return ct;
}


/*
 * Calculator的语法结构如下：
 * program:
 *     end
 *     expr_list end
 *
 * expr_list:
 *     expression print
 *     expression print expr_list
 *
 * expression:
 *     expression + term
 *     expression - term
 *
 * term:
 *     term / primary
 *     term * primary
 *     primary
 *
 * primary:
 *     number
 *     name
 *     name = expression
 *     -primary
 *     ( expression )
 *
 */
double expr(bool);

double prim(bool get){
    double left = prim(get);

    while(true){
        switch(ts.current().kind){
            case Kind::number:{
                double v = ts.current().number_value;
                ts.get();
                return v;
            }
            case Kind::name:{
                double &v = table[ts.current().string_value];
                if(ts.get().kind == Kind::assign)
                    v = expr(true);
                return v;
            }
            case Kind::minus:
                return -prim(true);
            case Kind::lp:{
                double e = expr(true);
                if(ts.current().kind != Kind::rp)
                    return error("')' expected");
                ts.get();               //吃掉右括号')'
                return e;
            }
            default:
                return error("primary expected");
        }
    }
}

int main(){

    while(true){
        ts.get();
        if(ts.current().kind == Kind::end)
            break;
        cout << ts.current();
    }
}
