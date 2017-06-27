#include "Pass1ListFile.h"
#include <fstream>
#include "Pass1.h"
#include"Instruction.h"
#include<bits/stdc++.h>
using namespace std ;
std::ofstream outfile ("List File.txt");

Pass1ListFile::Pass1ListFile( vector < Instruction >  x ,map <string,string> y, unordered_map<string , string> literal, map <string,string> expre)
{
createListFile(x);
createSymbolTable(y, expre);
createLiteralTable(literal);

}

Pass1ListFile::~Pass1ListFile()
{
    //dtor
}


void Pass1ListFile::createListFile(vector<Instruction> instructionList){



  //  Pass1 pass1;
   // vector<Instruction> instructionList=pass1.getInstructionList();
   string temp="";
   bool flag=false;
    for(int i = 0 ; i < instructionList.size() ; i++ ) {
        Instruction inst = instructionList[i];
        if(inst.isError()){
            outfile<<inst.getCommand()<<endl;
            if(inst.getError()!="")
                outfile<<inst.getError()<<endl;

        }else{
            temp=inst.getLabel();

            if(i>0&&instructionList[i-1].getOperator()!="LTORG"&&temp[0]=='='){
            string b=instructionList[i-1].getLabel();
            if((b!=""&&b[0]!='=')||b==""){
                Instruction a=instructionList[instructionList.size()-1];
                outfile<<inst.getAddress()<<"   "<<a.getCommand()<<endl;
                flag=true;

            }

            }
            if(flag&&inst.getOperator()=="END"){
                continue;
            }
            outfile<<inst.getAddress()<<" "<<inst.getCommand()<<"         "<<inst.getObjectCode()<<endl;
        }
    }

}
void Pass1ListFile::createSymbolTable(std::map<string,string>y,map <string,string> expre){
    outfile<<"-----------------------------------------------------------"<<endl;
    outfile<<"***SYMBOL TABLE ***"<<endl;

    for(auto &i:y){
        outfile<<i.first<<"             "<<i.second<<endl;
    }
    outfile<<"-----------------------------------------------------------"<<endl;
    outfile<<"***Relative/Absolute TABLE ***"<<endl;
    for(auto &i:expre){
        outfile<<i.first<<"             "<<i.second<<endl;
    }

}

void Pass1ListFile::createLiteralTable(unordered_map<string , string> literalTable){
    outfile<<"-----------------------------------------------------------"<<endl;
    outfile<<"***LITERAL TABLE "<<endl;

    typedef std::unordered_map<std::string, std::string>::iterator it_type;
    for(it_type iterator = literalTable.begin(); iterator != literalTable.end(); iterator++)
    {
        string literal = iterator->first;
        string address = iterator->second;
        outfile<<literal<<"             "<<address<<endl;
    }

}
