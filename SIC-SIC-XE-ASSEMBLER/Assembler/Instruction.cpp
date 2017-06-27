#include "Instruction.h"

Instruction::Instruction()
{
    //ctor
}

Instruction::~Instruction()
{
    //dtor
}

void Instruction::setFormat(int x){
	format = x;
}

int Instruction::getFormat() {
	return format;
}

void Instruction::setLabel(string x){
	label = x;
	hasLabels = true;
}

string Instruction::getLabel() const{
	return label;
}

bool Instruction::hasLabel() {
    return hasLabels;
}

void Instruction::setOperator(string str){
	_operator = str;
}

string Instruction::getOperator() const {
	return _operator;
}

void Instruction::setOperand(string x){
	hasOp = true;
	operand = x;
}

string Instruction::getOperand() const {
	return operand;
}

bool Instruction::hasOperand() const{
	return hasOp;
}

void Instruction::setComment(string x){
    commentFlag = true;
	comment = x;
}

string Instruction::getComment() const{
	return comment;
}

bool Instruction::isComment() const{
	return commentFlag;
}

void Instruction::setBlank() {
    blankFlag = true;
}
bool Instruction::isblank() const {
    return blankFlag;
}

void Instruction::setError(string str){
	error= str;
	errorFlag = true;
}

bool Instruction::isError() const {
	return errorFlag;
}

string Instruction::getError() const{
	return error;
}

void Instruction::setCommand(string line){
	command = line;
}

string Instruction::getCommand() const {
	return command;
}

void Instruction::setAddress(string x){
	address = x;
}
string Instruction::getAddress() const{
	return address;
}

bool Instruction::isDirective() const{
	return Directive;
}

 string Instruction:: getObjectCode(){
    return objectCode;

 }
void Instruction::setObjectCode(string x){

    objectCode=x;

}
