#ifndef OBJECTCODEFILE_H
#define OBJECTCODEFILE_H
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include"Instruction.h"
using namespace std;

class ObjectCodeFile
{
    public:
        ObjectCodeFile(std::vector < Instruction > x,std::vector < string > modificationRecords);
        virtual ~ObjectCodeFile();
        void writeObjectCodeFile(std::vector < Instruction > x,std::vector < string > modificationRecords);
    protected:
    private:
    string startAdd,endAdd,length,name="      ";
    string getLength(string x,string y);
    string formatOutput6(string result);
    int startIndex,endIndex;
    string formatOutput2(string result);
};

#endif // OBJECTCODEFILE_H
