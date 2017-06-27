#ifndef PASS1LISTFILE_H
#define PASS1LISTFILE_H
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include"Instruction.h"
using namespace std;
class Pass1ListFile
{
    public:
        Pass1ListFile( vector < Instruction >  x ,map <string,string> y, unordered_map<string , string> literal, map <string,string> exp);
        virtual ~Pass1ListFile();
        void createLiteralTable(unordered_map<string , string> literal);
        void createListFile(vector<Instruction> instructionList);
        void createSymbolTable(std::map<string,string>y, map <string,string> expr);
    protected:
    private:
};

#endif // PASS1LISTFILE_H
