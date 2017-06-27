#include "Pass1.h"
#include "Pass1ListFile.h"
#include"Pass2.h"
using namespace std;
int main(int argc, char* const argv[])
{
   string fileName ="2.txt";
    if(argc>1)
        fileName = argv[1];
    int type=1;
    Pass1 pass1(fileName,type);
    Pass2 Pass2(pass1.getInstructionList(),pass1.symbolTable,pass1.sym_exp,pass1.gent,pass1.literalTable);



}
