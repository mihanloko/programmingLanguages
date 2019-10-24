//
// Created by mikhail on 18.10.2019.
//

#ifndef SCANNER_SYNTAXDIAGRAMS_H
#define SCANNER_SYNTAXDIAGRAMS_H

#include "Scanner.h"
#include "defs.h"
#include <string>

#define isConst (type == HEX || type == DEC || type == OCT)

class SyntaxDiagrams {
public:
    explicit SyntaxDiagrams(Scanner* scanner);
    void program();
    void main();
    void classDefinition();
    void variableDefinition();
    void classContent();
    void variableList();
    void variable();
    void expression();
    void operatorsAndVariables();
    void operators();
    void oneOperator();
    void simpleOperator();
    void compoundOperator();
    void operatorIf();
    void operatorReturn();
    void a1();
    void a2();
    void a3();
    void a4();
    void a5();
    void a6();
    void a7();
    void assign();
    void arrayAccess();
    void classAccess();


private:
    Scanner* scanner;
};


#endif //SCANNER_SYNTAXDIAGRAMS_H
