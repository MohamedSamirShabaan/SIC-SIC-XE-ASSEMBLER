#include<regex>
#include <bits/stdc++.h>
#include <vector>
#include <queue>
#include <iostream>
#include <string.h>
#ifndef PARSER_H
#define PARSER_H
#include <bits/c++io.h>
#include "Instruction.h"
#include "GeneralExpression.h"

#include "Check.h"

using namespace std ;
class Parser
{
public:
    Parser();
    virtual ~Parser();
    string removeFirstSpaces( string line );
    string getUpperCase( string line );
    string getFirstWord( string line );
    GeneralExpression gen;
    string RemoveFirstWord( string line );
    map < string , string > symbolTable;
    unordered_map <string , string > literalTable;
    void solve(int format, string newLine,string label,string oper);
    void setLiteralTable(unordered_map<string , string > literal);
    void setSymbolTable(map < string , string >);
    map <string, string> sym_exp_table;
    map < string , string > getSymbolTable();
    unordered_map < string , string > getLiteralTable();
    vector<string>literalList;
    vector<string>getLiteralList();
    void insertIntoLiteralTable(string operands);
    void set_exp_table(map<string , string> x);
    map<string , string> get_exp_table();
    Instruction getInstruction();
    bool endFlag=false;
    bool startFlag = false;
    void parse(string line, int format, GeneralExpression generat, string LOCTR);
    void validateFixed(string line);
    queue<string> linePartsQueue;
    queue<string> lineRemQueue;

    int fileFormat , fixedFormat=0, freeFormat=1;
    string start = "";
    Check check;
    regex resw_operand_reg = regex( "^0$|^[1-9][0-9]*$");
    regex resb_operand_reg = regex("\\d{1,4}");
    regex start_operand_reg = regex("([A-F]|\\d){0,4}");
    regex end_operand_reg = regex("([A-Z]|\\d){0,4}");
    regex byte_operand_hexa_reg = regex("((X[\']([A-F]|\\d){0,14}[\']))");
    regex byte_operand_char_reg = regex("((c|C)'((\\s){0,1}(\\w){0,1}){0,15}')");
    regex word_operand_reg = regex("(-?\\d{1,4}(,-?\\d{1,4})*)");// here
    regex address_operand_reg = regex("(([A-F]|\\d){0,14})");
    regex e = regex( "^(\\w+)+[\\-\\*\\/\\+](\\w+)+[\\-\\*\\/\\+](\\w+)+[\\-\\*\\/\\+](\\w+)+[\\-\\*\\/\\+](\\w+)+[\\-\\*\\/\\+](\\w+)+");
    regex e1 = regex( "^[0-9]+");
    regex e2 = regex( "^[A-Z0-9_]+");
    regex e3 = regex( "^#[0-9]+");
    regex e4 = regex( "^#[A-Z0-9_]+");
    regex e5 = regex( "^[0-9]+\\,[xX]?");
    regex e6 = regex( "^[A-Z0-9_]+\\,[xX]?");
    regex e7 = regex( "^@[0-9]+");
    regex e8 = regex( "^@[A-Z0-9_]+");

protected:
private:
    string dec_to_hex(string str);
};

#endif // PARSER_H
