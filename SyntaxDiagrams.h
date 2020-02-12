//
// Created by mikhail on 18.10.2019.
//

#ifndef SCANNER_SYNTAXDIAGRAMS_H
#define SCANNER_SYNTAXDIAGRAMS_H

#include "Scanner.h"
#include "defs.h"
#include "Tree.h"
#include <string>

#define isConst (type == HEX || type == DEC || type == OCT || type == CONST)

class SyntaxDiagrams {
public:
    explicit SyntaxDiagrams(Scanner* scanner);
    bool program();
    void main();
    void classDefinition();
    void variableDefinition();
    void classContent();
    void variableList(Tree *typeDef);
    void variable(Tree *typeDef);
    Tree *expression();
    void operatorsAndVariables();
    void operators();
    void oneOperator();
    void simpleOperator();
    void compoundOperator();
    void operatorIf();
    void operatorReturn();
    Tree * a1();
    Tree * a2();
    Tree * a3();
    Tree * a4();
    Tree * a5();
    Tree * a6();
    Tree * a7();
    Tree * assign();
    Tree * arrayAccess();
    Tree * classAccess();


private:
    Scanner* scanner;
};


#endif //SCANNER_SYNTAXDIAGRAMS_H
