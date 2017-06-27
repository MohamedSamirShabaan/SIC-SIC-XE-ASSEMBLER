# SIC-SIC-XE-ASSEMBLER
The term project is to implement a (cross) assembler for (a subset of) SIC/XE assembler , written in C/C++ , producing code for the absolute loader used in the SIC/XE programming assignments.


# The output of the assembler include (at least):
The assembler is to execute by entering assemble
<source-file-name>
The source file for the main program for this phase is to
be named assemble.cpp
Object-code file whose format is the same as the one
described in the text book in section 2.1.1 and 2.3.5.
EQU and ORG statements.
Simple expression evaluation. A simple expression
includes simple (A<op> B) operand arithmetic, where
<op> is one of +,-,*,/ and no spaces surround the
operation, eg. A+B.
General expression evaluation.
Literals (Including LTORG) =C’<ASCII-TEXT>’ , =X’HEX-
TEXT’ , =<DECIMAL-TEXT> forms.
