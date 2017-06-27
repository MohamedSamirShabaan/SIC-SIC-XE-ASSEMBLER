#include "GeneralExpression.h"
#include<bits/stdc++.h>

using namespace std;


const char * expressionToParse ;

GeneralExpression::GeneralExpression()
{
    //ctor
}

GeneralExpression::~GeneralExpression()
{
    //dtor
}

char peek()
{
    return *expressionToParse;
}

char get()
{
    return *expressionToParse++;
}

double expression();

double number()
{
    double result = get() - '0';
    while (peek() >= '0' && peek() <= '9')
    {
        result = 10*result + get() - '0';
    }
    return result;
}

double factor()
{
    if (peek() >= '0' && peek() <= '9')
        return number();
    else if (peek() == '(')
    {
        get(); // '('
        double result = expression();
        get(); // ')'
        return result;
    }
    else if (peek() == '-')
    {
        get();
        return -expression();
    }
    return 0; // error
}

double term()
{
    double result = factor();
    while (peek() == '*' || peek() == '/')
        if (get() == '*')
            result *= factor();
        else
            result /= factor();
    return result;
}

double expression()
{
    double result = term();
    while (peek() == '+' || peek() == '-')
        if (get() == '+')
            result += term();
        else
            result -= term();
    return result;
}

string GeneralExpression:: evaluate(string s){

    string get = convert(s);

    if (get == ""){
           return "";
    }else {
           expressionToParse = get.c_str();
          int result = expression();

          std::stringstream sstream;
          sstream << std::hex <<  result;
          string temp = sstream.str();
          return temp;
    }
}

int GeneralExpression:: check_realtive(string s){
    regex e = regex( "^(\\w+)+[\\-\\*\\/\\+](\\w+)+[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*");
    smatch m;
    char operation [10];
    int counter = 0 , is_relateive = 0;
    for (int i = 0 ; i < s.size() ; i++){
        if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/'){
            operation[counter] = s[i];
            counter++;
        }
    }
   if(regex_search(s,m,e)){
        for(int i = 0 ; i < counter ; i++){
            string temp1 = m[i+1].str();
            string temp2 = m[i+2].str();
            if(i==0&&sym_exp_table[temp1]=="relative"){
                is_relateive++;
            }
            if(operation[i]=='*' ||operation[i]=='/'){
                if(sym_exp_table[temp1]=="relative"||sym_exp_table[temp2]=="relative"){
                    return -1;
                }
            }else if(operation[i]=='+' && sym_exp_table[temp2]=="relative"){
                is_relateive++;
            }else if(operation[i]=='-' && sym_exp_table[temp2]=="relative"){
                is_relateive--;
            }
        }
    }
    return is_relateive;
}

string GeneralExpression:: convert(string expression){
    regex e1 = regex( "[0-9]+");
    regex e2 = regex( "[A-Z]?[A-Z0-9_]*");
    string temp = "" ,  last = "";
    for (int i = 0 ; i < expression.size() ; i++){
        if(expression[i] == '-' || expression[i] == '+' || expression[i] == '/' || expression[i] == '*'){
            if (regex_match( temp , e1 )){
                last += temp;
            }else if (regex_match( temp , e2)){
                string str = symbol_table[temp];
                if (str == ""){
                    return "";
                }
                int dec;
                std::istringstream(str) >> std::hex >> dec;
                std::stringstream sstream;
                sstream <<  dec;
                last += sstream.str();
            }else {
                return "";
            }

            last += expression[i];
            temp = "";
        }else {
            temp += expression[i];
        }
    }
    if (temp != ""){
        if (regex_match( temp , e1 )){
            last += temp;
        }else if (regex_match( temp , e2)){
            string str = symbol_table[temp];
            if (str == ""){
                return "";
            }
            int dec;
            std::istringstream(str) >> std::hex >> dec;
            std::stringstream sstream;
            sstream <<  dec;
            last += sstream.str();
            }else {
                return "";
            }
        }

    return last;
}

void GeneralExpression::set_symbol_table(map < string , string > x){
    symbol_table = x;
}

map < string , string > GeneralExpression:: get_symbol_table(){
}

void GeneralExpression::set_exp_table(map < string , string > x){
    sym_exp_table = x;
}

 map < string , string > GeneralExpression::get_exp_table(){
}
