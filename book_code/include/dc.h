#ifndef Calculator_H
#define Calculator_H

#include<iostream>
#include<string>
#include<cctype>
#include<sstream>
#include<map>

using std::string;
using std::ostream;
using std::istream;
using std::map;

namespace Lexer{
    /*
     * Token的类型
     */
    enum class Kind : char {
        name, number, end,
        plus='+', minus='-', mul='*', div='/', print=';', assign='=',
        lp='(', rp=')'
    };

    /*
     * Token结构体，string_value保存name token的值，number_value保存number token的值
     */
    struct Token {
        Kind kind;
        string string_value;
        double number_value;
    };

    /*
    * 对Token的格式化输出
    */
    ostream& operator<<(ostream &os, Token &t);

    /*
     * 对Token输入读取的处理器
     */
    class Token_stream{
        public:
            Token_stream(istream &s):ip{&s}, owns{false}{ ct = {Kind::end};}
            Token_stream(istream *p):ip{p}, owns{true}{ ct = {Kind::end}; }
            
            ~Token_stream(){ close(); }

            Token get();                //获取下一个token
            Token& current();           //获取当前的token的引用

            void set_input(istream &s) { close(); ip = &s; owns=false;}
            void set_input(istream *p) { close(); ip = p; owns=true;}

        private:
            void close() { if(owns) delete ip; }

            istream *ip;                //input stream的指针
            bool owns;                  //这个Token_stream是否拥有ip
            Token ct;                   //current token
    };

    extern Token_stream ts;
}


namespace Error{
    double error(const string &s);
    extern int no_of_errors;
}

namespace Table{
    extern map<string, double> table;
}

namespace Parser{
    double prim(bool);
    double term(bool);
    double expr(bool);
}

namespace Dirver{
    void calculate();
}

#endif
