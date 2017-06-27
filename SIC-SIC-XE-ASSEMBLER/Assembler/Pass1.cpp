#include "Pass1.h"
#include <iostream>
#include "Parser.h"
#include<string.h>
#include"Pass1ListFile.h"
using namespace std;
string  hLocationCounter = "0000";
Parser parser;
GeneralExpression general;
Pass1::Pass1(string fileName,int type)
{
//     ctor

    string line = "";
    int size = 0 ;

    if(readFile(fileName))
    {
        while(size < inputFile.size())
        {
            line = "";
            line = inputFile[size];
            parser.setSymbolTable(symbolTable);
            parser.set_exp_table(sym_exp);
            general.set_symbol_table(symbolTable);
            gent = general;
            parser.parse(line, type,general, hLocationCounter);       //0 ---> fixed   1-->free
            Instruction inst = parser.getInstruction();
            symbolTable = parser.getSymbolTable();
            sym_exp = parser.get_exp_table();
            general.set_symbol_table(symbolTable);
            general.set_exp_table(sym_exp);
            gent = general;
            if(inst.getOperator()=="LTORG")
            {
                inst.setAddress(hLocationCounter);
                instructionList.push_back(inst);
                insertAddressIntoLiteralTable();
            }
            if(inst.getOperator() == "END")
            {
                hasEnd = true ;
                insertAddressIntoLiteralTable();
            }
            if (inst.getOperator() == "EQU")
            {
                if(inst.getOperand() == "*"){
                    symbolTable[inst.getLabel()] = hLocationCounter;
                }
                inst.setAddress(hLocationCounter);
            }
            else if(!inst.isError() && inst.getOperator()!="ORG")
            {
                inst.setAddress(hLocationCounter);
                if( inst.getOperator() == "START")
                {
                    hLocationCounter = inst.getOperand();
                    inst.setAddress(hLocationCounter);
                    if( inst.hasLabel()  )
                    {
                        if(inst.getLabel().size()!=0)
                        {
                            symbolTable[inst.getLabel()] = hLocationCounter;
                        }
                    }
                }
                else if(inst.getOperator() == "END")
                {
                    inst.setAddress(hLocationCounter);
                }
                else
                {
                    if( inst.hasLabel()  )
                    {
                        if(inst.getLabel().size()!=0)
                        {
                            symbolTable[inst.getLabel()] = hLocationCounter;

                        }
                    }

                        hLocationCounter = incrementLC(  inst.getFormat() );

                }

            }
            else if(inst.getOperator() == "ORG")
            {
                inst.setAddress(hLocationCounter);
                if(inst.getOperand() == "" || inst.getError() == "illegal label")
                {
                    // do nothing
                }
                else if(inst.getError()=="undefined symbol in operand" || inst.getError() == "INVALID EXPRESSION!")
                {
                    hLocationCounter = "0";
                }
                else if(inst.getError() != "" && inst.getError().substr(0,3) == "get")
                {
                    string address = inst.getError().substr(3 , inst.getError().size());
                    hLocationCounter = address;
                    inst.setError("");
                }
                else if (inst.getOperand() != "*")
                {
                    hLocationCounter = symbolTable[inst.getOperand()];
                }
            }
            if(inst.getOperator()!="LTORG")
            {
                instructionList.push_back(inst);
            }

            size++;
        }
        Instruction inst;
        if(!hasEnd)
        {

            inst.setError("hasNoEnd");
            instructionList.push_back(inst);
        }
        //Pass1ListFile pass1ListFile(instructionList,symbolTable);


    }


}


bool Pass1:: readFile(string fileName)
{
    ifstream file(fileName,ios::in);
    std::string read;
    if (!file.is_open())
    {
        printf("Invalid file\n");
        return false ;
    }
    else
    {
        while (getline(file, read))
        {

            inputFile.push_back(read);

        }
    }
    return true;
}
string Pass1::incrementLC( int add )
{
    int n;
    std::istringstream(hLocationCounter) >> std::hex >> n;
    std::stringstream sstream;
    sstream << std::hex << n + add;
    string result = sstream.str();
    switch (result.size())
    {
    case 1 :
        result = "000"+result;
        break;
    case 2 :
        result = "00"+result;
        break;
    case 3 :
        result ="0"+result;
        break;
    }
    return result;
}
void Pass1::writeListingFile()
{
    for(int i = 0 ; i < instructionList.size() ; i++ )
    {
        Instruction inst = instructionList[i];

    }
}
vector<Instruction> Pass1:: getInstructionList()
{
    return instructionList;
}
void Pass1::insertAddressIntoLiteralTable()
{
    literalTable = parser.getLiteralTable();
    literalList = parser.getLiteralList();
    for(int i = 0 ; i<literalList.size();i++)
    {
        string literal =literalList[i];
        if(literalTable[literal] == "")
        {
            literalTable[literal]= hLocationCounter;
            Instruction inst ;
            inst.setCommand(literal+"   Byte   "+literal.substr(1,literal.size()));
            inst.setAddress(hLocationCounter);
            inst.setOperand(literal.substr(1,literal.size()));
            inst.setOperator("BYTE");
            inst.setLabel(literal);

            if(literal[1]=='C')
            {
                hLocationCounter = incrementLC(literal.size()-4);
                inst.setFormat(literal.size()-3);
            }
            else if(literal[1]=='X')
            {
                hLocationCounter = incrementLC((literal.size()-4)/2);
                inst.setFormat((literal.size()-3)/2);
            }
            else{ //*
                string address = literal.substr(2,literal.size());
                int cou = 0;
                for(int i=0; i< address.size(); i++){
                    if(address[i] != '0') cou ++;
                }
                if(cou % 2 == 1) cou++;
                hLocationCounter = incrementLC(cou/2);
                inst.setFormat(cou/2);
            }
            instructionList.push_back(inst);
        }
        // iterator->second = value
        // Repeat if you also want to iterate through the second map.
    }
    parser.setLiteralTable(literalTable);
}
Pass1::~Pass1()
{
//     dtor
}
