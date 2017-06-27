#include "Pass2.h"
#include "GeneralExpression.h"
#include"Instruction.h"
#include"Check.h"
#include<bits/stdc++.h>
#include<regex>
#include "Pass1ListFile.h"
#include <stdlib.h>     /* atoi */
#include "ObjectCodeFile.h"

using namespace std;
vector<Instruction> instructions;
Pass2::Pass2(vector<Instruction> instructionList,map<string,string>x,map<string , string> y , GeneralExpression ger,unordered_map<string , string> literal)
{
    opTableFormat2=readOpTable.getOpTableFormat2();
    opTableFormat3=readOpTable.getOpTableFormat3();
    registers["A"]="0";
    registers["X"]="1";
    registers["L"]="2";
    registers["B"]="3";
    registers["S"]="4";
    registers["T"]="5";
    registers["F"]="6";
    symbolTable=x;
    literalTable = literal;
    sym_exp=y;
    g = ger;
    instructions=validateInstruction(instructionList);
    modification_record = vector <string> ();
}


Pass2::~Pass2()
{
    //dtor
}

vector<Instruction> Pass2::validateInstruction(vector<Instruction> instructionList)
{
    regex e = regex( "^(\\w+)+[\\-\\*\\/\\+](\\w+)+[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*");
    regex word_operand_reg = regex("(-?\\d{1,4}(,-?\\d{1,4})*)");// here

    for(int i=0; i<instructionList.size(); i++)
    {
        newInstruction=Instruction();
        newInstruction=instructionList[i];
        if(newInstruction.getOperator() == "BASE")
        {
            base = true;
        }
        else if (newInstruction.getOperator() == "NOBASE")
        {
            base = false;
        }
        if(newInstruction.getOperator()=="BYTE")
        {
            string operand=newInstruction.getOperand();
            bool flagC=false;
            if(operand[0]=='C')
            {
                flagC=true;

            }
            operand=operand.substr(2,operand.size()-3);
            if(flagC)
            {
                string hexStr;
                stringstream sstream;
                for(int i=0; i<operand.size(); i++)
                {

                    sstream<< hex << (int)operand[i];
                    hexStr= sstream.str();
                }
                operand=hexStr;
            }

            newInstruction.setObjectCode(operand);
            instructionList[i]=newInstruction;
        }
        else if (newInstruction.getOperator()=="WORD"&&regex_match(newInstruction.getOperand(),word_operand_reg))
        {
            string operand=newInstruction.getOperand();
            operand=dec_to_hex(operand);
            switch(operand.size())
            {
            case 1 :
                operand="00000"+operand;
                break;
            case 2 :
                operand = "0000"+operand;
                break;
            case 3 :
                operand="000"+operand;
                break;
            case 4 :
                operand="00"+operand;
                break;
            case 5 :
                operand="0"+operand;
                break;
            }
            newInstruction.setObjectCode(operand);
            instructionList[i]=newInstruction;

        }
        if(newInstruction.getFormat()==2&&check.directives.count(newInstruction.getOperator())==0)
        {
            checkFormat2();
            instructionList[i]=newInstruction;
        }
        else if (newInstruction.getOperator()=="RSUB")
        {
            newInstruction.setObjectCode("4F0000");
            instructionList[i]=newInstruction;

        }
        else if (newInstruction.getFormat()==3&&check.directives.count(newInstruction.getOperator())==0)
        {

            if (newInstruction.getOperator() == "LDB")
            {
                baseRelative = newInstruction.getOperand();
            }
            if(newInstruction.getOperand()!="")
            {
                checkFormat3();
            }
            instructionList[i]=newInstruction;
        }
        else if ((newInstruction.getFormat()==4&&check.directives.count(newInstruction.getOperator())==0))
        {

            if (newInstruction.getOperator() == "LDB")
            {
                baseRelative = newInstruction.getOperand();
            }
            if(newInstruction.getOperand()!="")
            {
                checkFormat4();
            }
            instructionList[i]=newInstruction;
        }
        else if(newInstruction.getOperator() == "WORD")
        {
            string tempOperand = newInstruction.getOperand();
            if (regex_match( tempOperand , e))
            {
                string evaluated = g.evaluate(tempOperand);
                if (evaluated != "")
                {
                    string operand= evaluated;
                    operand=dec_to_hex(operand);
                    switch(operand.size())
                    {
                    case 1 :
                        operand="00000"+operand;
                        break;
                    case 2 :
                        operand = "0000"+operand;
                        break;
                    case 3 :
                        operand="000"+operand;
                        break;
                    case 4 :
                        operand="00"+operand;
                        break;
                    case 5 :
                        operand="0"+operand;
                        break;
                    }
                    newInstruction.setObjectCode(operand);
                    int is_relative = g.check_realtive(tempOperand);
                    if (is_relative == 0)
                    {
                        tempOperand = evaluated;
                        newInstruction.setOperand(tempOperand);
                        instructionList[i]=newInstruction;
                        if(newInstruction.getLabel() != "")
                        {
                            sym_exp[newInstruction.getLabel()] = "absolute";
                            add_modification_record(newInstruction.getAddress(),"06");

                        }
                    }
                    else if(is_relative == 1)
                    {
                        newInstruction.setError("WORD CAN NOT HAVE RELATIVE EXPRESSION!!");
                        instructionList[i]=newInstruction;
                    }
                    else
                    {
                        newInstruction.setError("INVALID EXPRESSION!");
                        instructionList[i]=newInstruction;
                    }
                }
                else
                {
                    newInstruction.setError("INVALID EXPRESSION!");
                    instructionList[i]=newInstruction;
                }
            }
            else
            {
                // set error operant ya bta3 el validation 11.15 to 12
                // set error operant ya bta3 el validation 11.15 to 12
            }

        }
    }
    Pass1ListFile pass1ListFile(instructionList,symbolTable, literalTable, sym_exp);
    ObjectCodeFile object(instructionList,modification_record);
    return instructionList;       //by ahmed Eid

}
void Pass2::checkFormat2()
{

    if(newInstruction.getOperator()=="CLEAR"||newInstruction.getOperator()=="TIXR")
    {
        string temp=readOpTable.opTableFormat2[newInstruction.getOperator()];
        string registerCheck=registers[newInstruction.getOperand()];
        if(registerCheck=="")
        {
            newInstruction.setError("NOT VALID OPERAND");
        }
        else
        {
            temp+=registerCheck;
            temp+="0";
            newInstruction.setObjectCode(temp);
        }
    }
    else
    {
        string tempOperand=newInstruction.getOperand();
        if(std::count( tempOperand.begin(), tempOperand.end(), ',')==1)
        {
            if(tempOperand.size()==3)
            {
                string  registerCheck1=registers[tempOperand.substr(0,1)];
                string registerCheck2=registers[tempOperand.substr(2,2)];
                if(registerCheck1==""||registerCheck2=="")
                {
                    newInstruction.setError("NOT VALID OPERAND");
                }
                else
                {
                    string temp=readOpTable.opTableFormat2[newInstruction.getOperator()];
                    temp+=registerCheck1;
                    temp+=registerCheck2;
                    newInstruction.setObjectCode(temp);
                }
            }
            else
            {
                newInstruction.setError("NOT VALID OPERAND");
            }

        }
        else
        {
            newInstruction.setError("NOT VALID OPERAND");
        }
    }
}

void Pass2::checkFormat3()
{
    //regex add = regex( "^[0-9]*");
    regex add = regex( "^[0-9]+");
    regex e1= regex( "^#[0-9]*");
    regex e2= regex( "^#[A-Z0-9_]*");
    regex e3= regex( "^@[0-9]*");
    regex e4= regex( "^@[A-Z0-9_]*");
    regex e5=regex("^[A-Z0-9_]*");
    regex e6=regex("^[0-9]*,X");
    regex e7=regex ("^[A-Z0-9_]*,X");
    regex isLiteral = regex("=((c|C)'((\\s){0,1}(\\w){0,1}){0,15}')|=(X[\']([A-F]|\\d){0,14}[\'])");
    regex e = regex( "^(\\w+)+[\\-\\*\\/\\+](\\w+)+[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*");
    string tempOperand=newInstruction.getOperand();
    int flag = 0;
    if(tempOperand=="#"||tempOperand=="@")
    {
        flag=2;
        newInstruction.setError("INVALID OPERAND");
    }
    if(tempOperand == "*" )
    {
        flag=2;
        string address = newInstruction.getAddress();
        int dec;
        std::istringstream(address) >> std::hex >> dec;
        //   dec+=3;
        std::stringstream sstream;
        sstream << std::hex <<  dec;
        address = sstream.str();
        //address= dec_to_hex(address);
        string nixbpe="110000";
        switch(address.size())
        {
        case 1 :
            address="00"+address;
            break;
        case 2 :
            address = "0"+address;
            break;
        }
        //calculate your obj_code
        //string opj_code = create_opject_code(nixbpe,address);
        string opj_code = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()], nixbpe , address);
        if(opj_code != "" )
        {
            string location_counter = newInstruction.getAddress();
            add_modification_record(location_counter ,"03");
        }
        newInstruction.setObjectCode(opj_code);
        flag = 2;
    }
    else if (regex_match( tempOperand , e))
    {
        string evaluated = g.evaluate(tempOperand);
        if (evaluated != "")
        {
            int is_relative = g.check_realtive(tempOperand);
            if (is_relative == 0)
            {
                tempOperand = evaluated;
                int dec;
                std::istringstream(evaluated) >> std::hex >> dec;
                std::stringstream sstream;
                sstream <<  dec;
                tempOperand = sstream.str();
                flag = 1;
                if(newInstruction.getLabel() != "")
                {
                    sym_exp[newInstruction.getLabel()] = "absolute";
                    add_modification_record(newInstruction.getAddress(),"03");

                }
            }
            else if(is_relative == 1)
            {
                tempOperand = evaluated;
                int dec;
                std::istringstream(evaluated) >> std::hex >> dec;
                std::stringstream sstream;
                sstream <<  dec;
                tempOperand = sstream.str();
                flag = 1;
                if(newInstruction.getLabel() != "")
                {
                    sym_exp[newInstruction.getLabel()] = "relative";
                }
            }
            else
            {
                flag = 2;
                newInstruction.setError("INVALID EXPRESSION!");
            }
        }
        else
        {
            flag = 2;
            newInstruction.setError("INVALID EXPRESSION!");
        }
    }

    if(flag == 1 || regex_match( tempOperand , add ))
    {

        string address=tempOperand;
        address= dec_to_hex(address);
        if(address.size()>3)
        {
            newInstruction.setError("out of range!!");
        }
        string nixbpe = "110100";
        string opj_code = create_opject_code(nixbpe,address);
        opj_code = opj_code.substr(0,3)+opj_code.substr(5,opj_code.size());
        newInstruction.setObjectCode(opj_code);



        //In case of #
    }
    else if(flag == 0&& (regex_match( tempOperand , e1 )||regex_match( tempOperand , e2 )))
    {


        string operand=newInstruction.getOperand();
        //IN case of #1000
        if(regex_match( operand , e1 ))
        {
            string nixbpe="010000";
            string address=operand.substr(1,operand.size());
            std::stringstream sstream;
            sstream << std::hex <<  atoi (address.c_str());
            address= sstream.str();
            switch(address.size())
            {
            case 1 :
                address="00"+address;
                break;
            case 2 :
                address = "0"+address;
                break;
            }
            if(address.size()>3)
            {
                newInstruction.setError("OPERAND OUT OF RANGE");
            }
            else
            {
                string newObjectCode = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()], nixbpe ,address);
                newInstruction.setObjectCode(newObjectCode);
            }
            //IN CASE OF #ADSADS
        }
        else if (flag == 0&& regex_match( operand , e2 ))
        {

            string operand=newInstruction.getOperand();
            operand=operand.substr(1,operand.size());
            string address=symbolTable[operand];
            if (newInstruction.getOperator() == "LDB")
            {
                baseRelative = address;

            }
            if(address!="")
            {
                string location=newInstruction.getAddress();
                string temp = getInstructionAddress(location ,address , 0 , 3,true);
                if (temp == "")
                {
                    if (base == true) //BASE RELATIVE
                    {
                        temp = getInstructionAddress(baseRelative ,address , 1 , 0,true);
                        if(temp == "")
                        {
                            newInstruction.setError("DISPLACEMENT OUT OF RANGE");
                        }
                        else
                        {
                            string nixbpe = "010100";
                            string newObjectCode = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()], nixbpe , temp);
                            newInstruction.setObjectCode(newObjectCode);
                        }
                    }
                    else
                    {
                        newInstruction.setError("DISPLACEMENT OUT OF RANGE");
                    }

                }
                else  //PC RELATIVE
                {
                    string nixbpe = "010010";
                    string newObjectCode = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()]
                                           , nixbpe , temp);
                    newInstruction.setObjectCode(newObjectCode);
                }
            }
            else
            {
                newInstruction.setError("NOT VALID OPERAND");
            }
        }
        //IN CASE OF @
    }
    else if ( flag == 0 && (regex_match( tempOperand , e3 )||regex_match( tempOperand , e4 )))
    {
        string operand=tempOperand.substr(1,tempOperand.size());
        string address;
        bool isHex;
        if(regex_match( tempOperand , e3 ))
        {
            address=operand;    //@1000
            isHex=false;
        }
        else
        {
            address=symbolTable[operand];   //@ASDASD
            isHex=true;
        }
        if(address!="")         //ADDRESS OR FOUND IN SYMBOL TABLE
        {
            string location=newInstruction.getAddress();
            string temp = getInstructionAddress(location ,address , 0 , 3,isHex);
            if (temp == "")
            {
                if (base == true) //BASE RELATIVE
                {
                    temp = getInstructionAddress(baseRelative ,address , 1 , 0,isHex);
                    if(temp == "")
                    {
                        newInstruction.setError("DISPLACEMENT OUT OF RANGE");
                    }
                    else
                    {
                        string nixbpe = "100100";
                        string newObjectCode = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()], nixbpe , temp);
                        newInstruction.setObjectCode(newObjectCode);
                        add_modification_record(newInstruction.getAddress(),"03");
                    }
                }
                else
                {
                    newInstruction.setError("DISPLACEMENT OUT OF RANGE");
                }

            }
            else  //PC RELATIVE
            {
                string nixbpe = "100010";
                string newObjectCode = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()]
                                       , nixbpe , temp);
                newInstruction.setObjectCode(newObjectCode);
                add_modification_record(newInstruction.getAddress(),"03");
            }
        }
        else
        {
            newInstruction.setError("NOT VALID OPERAND");

        }


    }
    else if (flag == 0&& (regex_match( tempOperand , e5 )||regex_match(tempOperand,isLiteral)|| tempOperand.substr(0,2) == "=*"))
    {
        string address="";
        if(tempOperand[0]=='=')
        {
            address = literalTable[tempOperand];
        }
        else address=symbolTable[tempOperand];
        if(address!="")
        {
            string location=newInstruction.getAddress();
            string temp = getInstructionAddress(location ,address , 0 , 3,true);
            if (temp == "")
            {
                if (base == true) //BASE RELATIVE
                {
                    temp = getInstructionAddress(baseRelative ,address , 1 , 0,true);
                    if(temp == "")
                    {
                        newInstruction.setError("DISPLACEMENT OUT OF RANGE ");
                    }
                    else
                    {
                        string nixbpe = "110100";
                        string newObjectCode = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()], nixbpe , temp);
                        newInstruction.setObjectCode(newObjectCode);
                        if(regex_match( tempOperand , e5 )&&sym_exp[tempOperand]=="absolute")
                        {
                            add_modification_record(newInstruction.getAddress(),"03");
                        }

                    }
                }
                else
                {
                    newInstruction.setError("DISPLACEMENT OUT OF RANGE");
                }

            }
            else  //PC RELATIVE
            {
                string nixbpe = "110010";
                string newObjectCode = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()]
                                       , nixbpe , temp);
                newInstruction.setObjectCode(newObjectCode);
                if(regex_match( tempOperand , e5 )&&sym_exp[tempOperand]=="absolute")
                {
                    add_modification_record(newInstruction.getAddress(),"03");
                }
            }
        }
        else
        {
            newInstruction.setError("NOT VALID OPERAND");
        }

    }

    else if(flag == 0 && regex_match( tempOperand , e6 ))     //1000,x
    {
        string address=tempOperand;
        address= dec_to_hex(address);
        if(address.size()>3)
        {
            newInstruction.setError("out of range!!");
        }
        string nixbpe = "111000";
        string opj_code = create_opject_code(nixbpe,address);
        opj_code = opj_code.substr(0,3)+opj_code.substr(5,opj_code.size());

        newInstruction.setObjectCode(opj_code);
    }
    else if(flag == 0 && regex_match(tempOperand,e7) )    // hamada ,x
    {
        string operand=tempOperand.substr(0,tempOperand.size()-2);
        string address=symbolTable[operand];
        if(address!="")
        {
            string location = newInstruction.getAddress();
            string temp = getInstructionAddress(location ,address , 0 , 3,true);
            if (temp == "")
            {
                if (base == true)
                {
                    temp = getInstructionAddress(baseRelative ,address , 1 , 0,true);
                    if(temp == "")
                    {
                        newInstruction.setError("DISPLACEMENT OUT OF RANGE");
                    }
                    else
                    {
                        string nixbpe = "111100";
                        string newObjectCode = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()], nixbpe , temp);
                        newInstruction.setObjectCode(newObjectCode);
                        if(sym_exp[operand]=="absolute")
                        {
                            add_modification_record(newInstruction.getAddress(),"03");
                        }
                    }
                }
                else
                {
                    newInstruction.setError("DISPLACEMENT OUT OF RANGE");
                }
            }
            else
            {
                string nixbpe = "111010";
                string newObjectCode = concatinateInstruction(readOpTable.opTableFormat3[newInstruction.getOperator()], nixbpe , temp);
                newInstruction.setObjectCode(newObjectCode);
                if(sym_exp[operand]=="absolute")
                {
                    add_modification_record(newInstruction.getAddress(),"03");
                }
            }


        }
        else
        {
            newInstruction.setError("INVALID OPERAND");

        }

    }
    else if(flag != 2)
    {
        newInstruction.setError("INVALID OPERAND");
    }
}

void Pass2::checkFormat4()
{
    regex e1 = regex( "^[0-9]*");
    regex e2 = regex( "^[A-Z0-9_]*");
    regex e3 = regex( "^#[0-9]*");
    regex e4 = regex( "^#[A-Z0-9_]*");
    regex e5 = regex( "^[0-9]*\\,[xX]?");
    regex e6 = regex( "^[A-Z0-9_]*\\,[xX]?");
    regex e7 = regex( "^@[0-9]*");
    regex e8 = regex( "^@[A-Z0-9_]*");
    regex isLiteral = regex("=((c|C)'((\\s){0,1}(\\w){0,1}){0,15}')|=(X[\']([A-F]|\\d){0,14}[\'])");
    regex e = regex( "^(\\w+)+[\\-\\*\\/\\+](\\w+)+[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*[\\-\\*\\/\\+]*(\\w+)*");
    string temp_operand=newInstruction.getOperand();
    string nixbpe = "", operand = "", address = "";
    int flag = 0;
    if(temp_operand=="#"||temp_operand=="@")
    {
        flag=2;
        newInstruction.setError("INVALID OPERAND");
    }
    if(temp_operand == "*" )
    {
        flag=2;
        string address = newInstruction.getAddress();
        int dec;
        std::istringstream(address) >> std::hex >> dec;
        //  dec+=4;
        std::stringstream sstream;
        sstream << std::hex <<  dec;
        address = sstream.str();
        //address= dec_to_hex(address);
        nixbpe="110001";
        string opj_code = create_opject_code(nixbpe,address);
        if(opj_code != "" )
        {
            string location_counter = newInstruction.getAddress();
            add_modification_record(location_counter ,"05");
        }
        flag = 2;
    }
    else if (regex_match( temp_operand , e ))
    {
        string evaluated = g.evaluate(temp_operand);
        if (evaluated != "")
        {
            int is_relative = g.check_realtive(temp_operand);

            if (is_relative == 0)
            {
                temp_operand = evaluated;
                flag = 1;
                if(newInstruction.getLabel() != "")
                {
                    sym_exp[newInstruction.getLabel()] = "absolute";
                    add_modification_record(newInstruction.getAddress() ,"05");

                }
            }
            else if(is_relative == 1)
            {
                temp_operand = evaluated;
                flag = 1;
                if(newInstruction.getLabel() != "")
                {
                    sym_exp[newInstruction.getLabel()] = "relative";
                }
            }
            else
            {
                flag = 2;
                newInstruction.setError("INVALID EXPRESSION!");

            }
        }
        else
        {
            flag = 2;
            newInstruction.setError("INVALID EXPRESSION!");

        }
    }
    /**
        110001    +op  1000    address = HEX(1000)
    */
    if(flag == 1 || regex_match( temp_operand , e1 ))
    {
        address=temp_operand;
        if (flag == 0)address= dec_to_hex(address);
        nixbpe="110001";
        string opj_code = create_opject_code(nixbpe,address);
        if(opj_code != "" )
        {
            string location_counter = newInstruction.getAddress();
            add_modification_record(location_counter ,"05");

        }
    }
    /**
        110001    +op  label    address = Label_address
    */
    else if(flag == 0 && (regex_match( temp_operand , e2 ) ||regex_match(temp_operand,isLiteral) || temp_operand.substr(0,2) == "=*"))
    {
        operand=newInstruction.getOperand();
        if(operand[0]=='=')
            address = literalTable[operand];
        else
            address=symbolTable[operand];
        nixbpe="110001";
        string opj_code = create_opject_code(nixbpe,address);
        string location_counter = newInstruction.getAddress();
        add_modification_record(location_counter ,"05");
    }
    /**
        010001    +op  #4000    address =  HEX(4000)
    */
    else if(flag == 0 && regex_match( temp_operand , e3 ))
    {
        operand=newInstruction.getOperand();
        address=operand.substr(1,operand.size());
        address= dec_to_hex(address);
        nixbpe="010001";
        string opj_code = create_opject_code(nixbpe,address);
        string location_counter = newInstruction.getAddress();
        add_modification_record(location_counter ,"05");
    }
    /**
        010001    +op  #label    address = Label_address
    */
    else if(flag == 0 &&regex_match( temp_operand , e4) )
    {
        operand=newInstruction.getOperand();
        operand=operand.substr(1,operand.size());
        address=symbolTable[operand];
        nixbpe="010001";
        string opj_code = create_opject_code(nixbpe,address);
        string location_counter = newInstruction.getAddress();
        add_modification_record(location_counter ,"05");
    }
    /**
        111001    +op   1000,x    address = HEX(1000) + x
    */
    else if(flag == 0 &&regex_match( temp_operand , e5) )
    {
        operand=newInstruction.getOperand();
        operand=operand.substr(0,operand.size()-2);
        address=operand;
        address= dec_to_hex(address);
        nixbpe="111001";
        string opj_code = create_opject_code(nixbpe,address);
        string location_counter = newInstruction.getAddress();
        add_modification_record(location_counter ,"05");
    }
    /**
        111001    +op   label,x    address = label_address + x
    */
    else if(flag == 0 &&regex_match( temp_operand , e6))
    {
        operand=newInstruction.getOperand();
        operand=operand.substr(0,operand.size()-2);
        address=symbolTable[operand];
        nixbpe="111001";
        string opj_code = create_opject_code(nixbpe,address);
        string location_counter = newInstruction.getAddress();
        add_modification_record(location_counter ,"05");
    }
    /**
        100001    +op   /@1000    address = ??
    */
    else if(flag == 0 &&regex_match( temp_operand , e7))
    {
        operand=newInstruction.getOperand();
        address=operand.substr(1,operand.size());
        address= dec_to_hex(address);
        nixbpe="100001";
        string opj_code = create_opject_code(nixbpe,address);
        string location_counter = newInstruction.getAddress();
        add_modification_record(location_counter ,"05");
    }
    /**
        100001    +op   /@label    address = ??
    */
    else if(flag == 0 &&regex_match( temp_operand , e8) )
    {
        operand=newInstruction.getOperand();
        operand=operand.substr(1,operand.size());
        address=symbolTable[operand];
        nixbpe="100001";
        string opj_code = create_opject_code(nixbpe,address);
        string location_counter = newInstruction.getAddress();
        add_modification_record(location_counter ,"05");
    }
    else if(flag != 2)
    {
        newInstruction.setError("INVALID OPERAND");
    }

}

string Pass2::dec_to_hex(string str)
{
    std::stringstream sstream;
    sstream << std::hex <<  atoi (str.c_str());
    return sstream.str();
}

void Pass2:: add_modification_record(string str ,string format)
{
    string temp = "";
    int dec;
    std::istringstream(str) >> std::hex >> dec;
    if(format != "06")
    {
        dec++;
    }
    std::stringstream sstream;
    sstream << std::hex <<  dec;
    string address = sstream.str();
    switch(address.size())
    {
    case 1 :
        address="00000"+address;
        break;
    case 2 :
        address = "0000"+address;
        break;
    case 3 :
        address = "000"+address;
        break;
    case 4 :
        address = "00"+address;
        break;
    case 5 :
        address = "0"+address;
        break;
    }
    if(address.size() > 6)
    {
        newInstruction.setError("MODIFICATION RECORD ERROR!!");
    }
    temp = "M" + address + format;
    modification_record.push_back(temp);

}

string Pass2::create_opject_code(string nixbpe, string address)
{
    switch(address.size())
    {
    case 1 :
        address="0000"+address;
        break;
    case 2 :
        address = "000"+address;
        break;
    case 3 :
        address = "00"+address;
        break;
    case 4 :
        address = "0"+address;
        break;
    }
    if(address.size() > 5)
    {
        newInstruction.setError("OPERAND OUT OF RANGE");
        return "";
    }
    else
    {
        string op = newInstruction.getOperator();
        op = op.substr(1,op.size());
        string new_object_code = concatinateInstruction(readOpTable.opTableFormat3[op], nixbpe , address);
        if(new_object_code.size()==6)
        {
            new_object_code="00"+new_object_code;
        }
        else if(new_object_code.size()==7)
        {
            new_object_code="0"+new_object_code;
        }
        newInstruction.setObjectCode(new_object_code);
        return new_object_code;
    }
}

string Pass2::getInstructionAddress( string pc , string add , int flag, int format,bool isHex)
{
    int n;
    int m;
    if(isHex)
    {
        std::istringstream(add) >> std::hex >> m;
    }
    else
    {
        m = atoi (add.c_str());
    }
    std::stringstream sstream;
    std::istringstream(pc) >> std::hex >> n;
    sstream << std::hex <<  m - (n+format);
    if(flag==0 &&(m-(n+format))>=-2048 && (m-(n+format)) <= 2047)
    {
        string result = sstream.str();
        switch(result.size())
        {
        case 1 :
            result="00"+result;
            break;
        case 2 :
            result = "0"+result;
            break;
        }
        result = result.substr(result.size()-3 , result.size());
        return result;
    }
    else if(flag==1 &&(m-(n+format))>=0 && (m-(n+format)) <= 4096)
    {
        string result = sstream.str();
        switch(result.size())
        {
        case 1 :
            result="00"+result;
            break;
        case 2 :
            result = "0"+result;
            break;
        }
        result = result.substr(result.size()-3 , result.size());
        return result;
    }
    string result = "";
    return result;
}

vector<string> Pass2::get_modofication_record()
{
    return modification_record;
}

string Pass2::concatinateInstruction(string opCode , string nixbpe , string address)
{
    opCode = "0x"+opCode;
    stringstream ss;
    ss << hex << opCode;
    unsigned n;
    ss >> n;
    bitset<8> b(n);
    string result = b.to_string() ;
    result = result.substr(0,6);
    result += nixbpe;
    unsigned long long value = std::bitset<64>(result).to_ullong();
    std::stringstream sstream;
    sstream << std::hex << value;
    result = sstream.str();
    result = result + address ;
    switch(result.size())
    {
    case 4 :
        result="00"+result;
        break;
    case 5 :
        result = "0"+result;
        break;
    }
    return result ;

}
