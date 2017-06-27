#include "ReadOpCode.h"
#include<bits/c++io.h>
using namespace std;
ReadOpCode::ReadOpCode()
{
readFile();

}

ReadOpCode::~ReadOpCode()
{

}

 void ReadOpCode::readFile(){
    ifstream file("opCode.txt");
    string read;
    if(!file.is_open()){
        printf("OP Code Invalid file\n");
        return;
    }else{
        while (getline(file, read)){
            string delimiter = " ";
            string first = read.substr(0, read.find(delimiter));
            string second = read.substr( 9,1);
            string third = read.substr( 15,2);
            opTable[first] = third;
            if(second == "2"){
                    opTableFormat2 [first] = third;
            }else{
                opTableFormat3 [first] = third;
            }
        }
    }
 }
 map <string ,string > ReadOpCode::getOpTable(){
    return opTable;
 }
 map <string ,string > ReadOpCode::getOpTableFormat2(){
    return opTableFormat2;
 }
 map <string ,string > ReadOpCode::getOpTableFormat3(){
    return opTableFormat3;
 }
