#include "Check.h"
#include <regex>
#include <bits/c++io.h>
Check::Check()
{
    //ctor
   // read.readFile();
    opTable = read.getOpTable();
    opTableFormat2 = read.getOpTableFormat2();
    opTableFormat3 = read.getOpTableFormat3();
}
int Check::getFormat( string line ){
    if (isspace(line.at(line.size()-1)))
        {
            line=line.substr(0,line.size()-1);
        }

	if( line[0] == '+' ){
		line = line.substr(1,line.size());
		if( (opTableFormat3.find(line) == opTableFormat3.end()) ){
			return 0 ;
		}
		else{
			return 4 ;
		}
	}


	if( directives.count(line) == 1 ){

		return 1 ;
	}
	if( opTable.find(line) == opTable.end() ){
		return 0 ;
	}

	if( opTableFormat2.find(line) == opTableFormat2.end() ){

		return 3 ;
	}
	if( opTableFormat3.find(line) == opTableFormat3.end() ){
		return 2 ;
	}


}
bool Check::isAcceptedLabel(string line){
    regex label_reg = regex("^[a-zA-Z_][a-zA-Z0-9_]*$" );
	bool correct = regex_match(line,label_reg);
	if (isspace(line.at(line.size()-1)))
        {
            line=line.substr(0,line.size()-1);
        }
	return correct&&(opTable.count(line)==0)&&(directives.count(line)==0);
}

void Check::setSymbolTable(map <string ,string > m){
    symbolTable = m;
}
Check::~Check()
{
    //dtor
}
