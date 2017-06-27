#include "ObjectCodeFile.h"
#include <bits/stdc++.h>
using namespace std;
ObjectCodeFile::ObjectCodeFile(std::vector < Instruction > x,std::vector < string > modificationRecords )
{
    writeObjectCodeFile(x,modificationRecords);

}

ObjectCodeFile::~ObjectCodeFile()
{
    //dtor
}

void ObjectCodeFile:: writeObjectCodeFile(std::vector < Instruction > x,std::vector < string > modificationRecords){
    std::ofstream outfile ("Object Code.txt");
    outfile<<"Object Code : "<<endl; outfile<<"--------------"<<endl; outfile<<"--------------"<<endl;outfile<<endl<<endl;
    bool validCode=false;
    for(int k=0;k<x.size();k++){
        Instruction temp=x[k];
        if(temp.error!=""){
           validCode=true;
           break;
        }

    }





   if(validCode){
        outfile<<"ERROR IN OBJECT CODE";
    }else{


        int i=0,j;
        for( i=0;i<x.size();i++){
            Instruction temp=x[i];
            if(!temp.isComment()&&temp.getOperator()=="START"){
                startAdd=temp.getAddress();
                startIndex=i;
                name=temp.getLabel();
                if(name==""){
                    name="      ";
                }
            }else if ((!temp.isComment()&&temp.getOperator()=="END")){
                endAdd=temp.getAddress();
                endIndex=i;
            }
        }
        length=getLength(endAdd,startAdd);

         switch(name.size()){
                case 1 : name=name+"     ";
                        break;
                case 2 : name=name+"    ";
                        break;
                case 3 : name=name+"   ";
                        break;
                case 4 : name=name+"  ";
                        break;
                case 5 : name=name+" ";
                        break;
            }

        outfile<<"H^"<<name<<"^"<<formatOutput6(startAdd)<<"^"<<formatOutput6(length)<<endl;
        i=startIndex;
        for(;i<endIndex;i++){
            bool flag=false;
            j=i;
            int counter=60;
            vector < string > line;
            for(;j<endIndex;j++){


                 Instruction temp=x[j];

                 if(!temp.isblank()&&!temp.isComment()){
                     if(!temp.isComment()&&counter<=0){
                        break;
                     }
                     if(temp.getOperator()=="RESW"||temp.getOperator()=="RESB"){
                        flag=true;
                        break;

                     }
                     int format=temp.getFormat();
                     if((format==2&&counter-4>=0)||(format==3&&counter-6>=0||(format==4&&counter-8>=0))){
                        line.push_back(temp.objectCode);

                        counter-=temp.getFormat()*2;

                     }else if (temp.getOperator()=="WORD"&&counter-6>=0){
                        line.push_back(temp.objectCode);
                        counter-=6;
                     }else if(temp.getOperator()=="BYTE"){
                        string lenObjectCode=temp.getObjectCode();
                        if(counter-lenObjectCode.size()>=0){
                            line.push_back(temp.objectCode);
                            counter-=lenObjectCode.size();
                        }else{
                            break;
                        }

                     }else if(format==2||format==3||format==4||temp.getOperator()=="WORD"){
                        break;
                     }
                }
            }
            outfile<<"T^"<<formatOutput6(x[i].getAddress())<<"^"<<formatOutput2(getLength(x[j].getAddress(),x[i].getAddress()))<<"^";
            for(int k=0;k<line.size();k++){
               std::string temp=line[k];
                outfile<<temp<<"^";
            }
            outfile<<endl;
            int k=j+1;
            if(flag){
                while(true){
                    Instruction temp2=x[k];
                    if(temp2.getOperator()=="RESW"||temp2.getOperator()=="RESB"){
                        j++;
                        k++;

                    }else{
                        j++;
                        break;
                    }
                }
            }
            i=j-1;
        }
        for(int i=0;i<modificationRecords.size();i++){
            outfile<<modificationRecords[i]<<endl;
        }
        outfile<<"E^"<<formatOutput6(startAdd);
    }

}

string ObjectCodeFile::getLength(string x,string y){
    int n,m;
    std::stringstream sstream;

    std::istringstream(x) >> std::hex >> n;
    std::istringstream(y) >> std::hex >> m;
     sstream << std::hex <<n-m;
     string result = sstream.str();
    return result;
}

string ObjectCodeFile::formatOutput6(string result){
    switch(result.size()){
            case 1 : result="00000"+result;
                    break;
            case 2 : result = "0000"+result;
                    break;
            case 3 : result="000"+result;
                    break;
            case 4 : result = "00"+result;
                    break;
            case 5 : result="0"+result;
                    break;
        }
    return result;
}

string ObjectCodeFile::formatOutput2(string result){
    switch(result.size()){
            case 1 : result="0"+result;
                    break;

        }
    return result;
}
