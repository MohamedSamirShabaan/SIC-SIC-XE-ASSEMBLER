#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <string>

using namespace std;

class Instruction
{
    public:
        Instruction();
        virtual ~Instruction();
        bool nFlag, iFlag, xFlag, pFlag, bFlag, eFlag,errorFlag,hasOp,hasLabels,commentFlag,Directive,blankFlag;
        int value,format;
        string label="",error,_operator,comment,command,operand,address,objectCode;
        void setValue(int val);
        int getFormat()const;
        /*
          0<--- comment
          1<--- directive
          2<--- format 2
          3<--- format 3
          4<--- format 4

            */
        string getObjectCode();
        void setObjectCode(string x);
          void setFormat(int x);
          int getFormat() ;
          void setLabel(string x);
          string getLabel() const;
          bool hasLabel();
          void setOperator(string str);
          string getOperator() const;
          void setOperand(string x);
          string getOperand() const;
          bool hasOperand() const;
          void setComment(string x);
          string getComment() const;
          bool isComment() const;
          void setBlank();
          bool isblank() const;
          void setError(string str);
          bool isError() const;
          string getError() const;
          void setCommand(string line);
          string getCommand() const;
          void setAddress(string x);
          string getAddress() const;
          bool isDirective() const;

//        string getLabel() const;
//        void setComment(string x);
//        void setFormat(int val);
//        void setOperator(string op);
//        string getOperator() const;
//        void setError(string err);
//        bool isError() const;
//        string getError() const;
//        void setCommand(string arg);
//        string getCommand() const;
//        string getOperand() const;
//        void setOperand(string op);
//        bool hasOperand() const;
//        void setAddress(int addr);
//        int getAddress() const;
//        bool isComment() const;
//        bool isblank() const;
//        void setBlank();
//        void setLabel(string x);
//        string getComment() const;
//        bool isDirective() const;
    protected:
    private:
};

#endif // INSTRUCTION_H
