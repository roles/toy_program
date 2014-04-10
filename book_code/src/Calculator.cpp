#include"Calculator.h"

using namespace Lexer;
using Table::table;
using namespace Error;

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

double Error::error(const string &s){
    no_of_errors++;
    cerr << "error: " << s << endl;
    return 1;
}

namespace Lexer{

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
}

namespace Parser{

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
     * expression:              //expression + term作为expression是一种自身递归的形式
     *     expression + term
     *     expression - term
     *     term
     *
     * term:
     *     term / primary
     *     term * primary
     *     primary
     *
     * primary:                 //在primary中用到expression是一种循环递归的方式
     *     number
     *     name
     *     name = expression
     *     -primary
     *     ( expression )
     * ------------------------------
     * prim, term, expr分别对应primary, term, expression的解析函数
     * 这些函数的get参数判断进行解析前是否需要从Token_stream中读取下一个token
     * 默认的情况下是不需要的，因为在main函数开始的时候已经进行了ts.get()
     * 但是在进行局部解析的时候需要针对情况决定是否读取.
     * 处理的方式基本跟上面的语法结构解析相似
     *
     * 每一次switch都是针对ts.current()是因为在这些函数处理后都会读下一个token
     *
     */
    double prim(bool get){
        if(get)
            ts.get();

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
                              ts.get();                   //吃掉右括号')'
                              return e;
                          }
            default:
                          return error("primary expected");
        }
    }

    double term(bool get){
        double left = prim(get);

        for(;;){                            //死循环用以处理连乘连除的情况
            switch(ts.current().kind){
                case Kind::mul:
                    left *= prim(true);
                    break;
                case Kind::div:             //除0判断
                    if(auto d = prim(true)){
                        left /= d;
                        break;
                    }
                    return error("divide by 0");
                default:
                    return left;
            }
        }
    }

    double expr(bool get){
        double left = term(get);

        for(;;){
            switch(ts.current().kind){
                case Kind::plus:
                    left += term(true);
                    break;
                case Kind::minus:
                    left -= term(true);
                    break;
                default:
                    return left;
            }
        }
    }
}

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
