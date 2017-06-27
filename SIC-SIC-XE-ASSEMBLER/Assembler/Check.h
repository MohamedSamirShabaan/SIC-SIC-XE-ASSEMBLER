#include<regex>
#include "ReadOpCode.h"
#include <bits/stdc++.h>
#ifndef CHECK_H
#define CHECK_H

using namespace std;
class Check
{
    public:
        Check();
        virtual ~Check();
        int getFormat( string line );
        map <string ,string > opTable;
        map <string ,string > symbolTable;
        map <string ,string > opTableFormat2;
        map <string ,string > opTableFormat3;
        void setSymbolTable(map <string ,string >);

        ReadOpCode read ;
        bool isAcceptedLabel(string line);
        unordered_set<string> directives = unordered_set<string>( { "WORD" , "BYTE" , "RESW" , "RESB" , "START" , "END","BASE" , "ORG" ,"EQU" , "NOBASE" , "LTORG" } ) ;
    protected:
    private:
};

#endif // CHECK_H
