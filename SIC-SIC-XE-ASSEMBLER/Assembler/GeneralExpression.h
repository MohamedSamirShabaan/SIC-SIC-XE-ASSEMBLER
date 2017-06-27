#ifndef GENERALEXPRESSION_H
#define GENERALEXPRESSION_H
#include <bits/stdc++.h>
using namespace std;
class GeneralExpression
{
    public:
        GeneralExpression();
        virtual ~GeneralExpression();
        map <string, string> sym_exp_table;
        map <string, string> symbol_table;
        string evaluate(string s);
        int check_realtive(string s);
        string convert(string s);
        void set_symbol_table(map < string , string > x);
        map < string , string > get_symbol_table();
        void set_exp_table(map < string , string >x);
        map < string , string > get_exp_table();

    protected:
    private:
};

#endif // GENERALEXPRESSION_H
