#include "dc.h"

using namespace Lexer;
using namespace Table;
using namespace Error;

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
double Parser::prim(bool get){
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

double Parser::term(bool get){
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

double Parser::expr(bool get){
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
