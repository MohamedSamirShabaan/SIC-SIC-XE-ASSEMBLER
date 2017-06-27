#ifndef PASS2_H
#define PASS2_H
#include"Instruction.h"
#include"bits/stdc++.h"
#include "Check.h"
#include "GeneralExpression.h"
using namespace std;


class Pass2
{
    public:

        map<string,string> opTableFormat2;
        map<string,string> opTableFormat3;
        ReadOpCode readOpTable;
        string baseRelative ;
        bool base = false;
        Instruction newInstruction;
        Pass2(vector<Instruction> instructionList,map<string,string>x,map<string , string> y , GeneralExpression ger,unordered_map<string , string> litral);
        virtual ~Pass2();
        vector<Instruction> validateInstruction(vector<Instruction> instructionList);
        void checkFormat2();
        void checkFormat3();
        void checkFormat4();
        GeneralExpression g;
        vector<string> get_modofication_record();
        string getInstructionAddress( string pc , string add , int flag, int format,bool isHex);
        string concatinateInstruction(string opCode , string nixbpe , string address);
        Check check;
        map<string,string> registers;
        map<string,string>symbolTable;
        unordered_map<string,string>literalTable;
        map<string , string>sym_exp;

    protected:
    private:
        string dec_to_hex(string str);
        string create_opject_code(string nixbpe, string address);
        vector <string> modification_record ;
        void add_modification_record(string str,string format);
};

#endif // PASS2_H
