#ifndef READOPCODE_H
#define READOPCODE_H
#include<string>
#include <map>
#include<bits/stdc++.h>
using namespace std;

class ReadOpCode
{
    public:
        ReadOpCode();
        virtual ~ReadOpCode();
        map <string ,string > opTable;
        map <string ,string > opTableFormat2;
        map <string ,string > opTableFormat3;
        void readFile();
        map <string ,string >  getOpTable();
        map <string ,string >  getOpTableFormat2();
        map <string ,string >  getOpTableFormat3();
    protected:
    private:

};

#endif // READOPCODE_H
