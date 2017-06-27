#ifndef PASS1_H
#define PASS1_H
#include <bits/stdc++.h>
#include "GeneralExpression.h"
#include <vector>
#include <iostream>
#include "Instruction.h"
using namespace std;
class Pass1
{
    public:
        Pass1(string fileName,int type);
        virtual ~Pass1();
        vector<string> inputFile;
        vector<Instruction>instructionList;
        map <string, string> symbolTable;
        map <string , string> sym_exp;
        unordered_map <string , string > literalTable;
        string incrementLC(int n);
        GeneralExpression gent;
        void insertAddressIntoLiteralTable();
        void writeListingFile();
        bool readFile(string fileName);
        bool hasEnd = false;
        vector<Instruction> getInstructionList();
        vector<string>literalList;
        int fixedFormat = 0, freeFormat = 1;

    protected:
    private:
};

#endif // PASS1_H
