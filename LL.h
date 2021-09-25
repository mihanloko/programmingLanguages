//
// Created by mikhail on 14.12.2019.
//

#ifndef SCANNER_LL_H
#define SCANNER_LL_H


#include "Scanner.h"
#include "defs.h"
#include "Tree.h"
#include "Triad.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

struct IfData {
    Triad *ifOperand;
    Triad *jmpTriad;
    int trueAddress;
    int falseAddress;
    int nopOperand;
};

class LL {
private:
    Scanner *scanner;
    int magazine[5000];
    int pos = 0;
    map<int, string> words;
    string lastLex;
    Tree* lastType;
    Tree* magazineTypes[5000];
    int typePos = 0;

    vector<IfData> ifData;
    vector<Triad *> triads;  //список сгенерированных триад
    vector<Operand *> operands;  //стек результатов

    template<typename TYPE> TYPE getTopValue(vector<TYPE> &st, const string &name);
public:
    LL(Scanner *scanner, map<int, string> &words);
    void analyze();

    void outOneOperand(Operand *operand);

    void outOneTriad(Triad *triad);

    void outTriads();

    void outOperands();

    string codeOperationToString(int code);

    void generateArithmeticTriad(int operation);

    Operand *getOperand();

    int getLastTriadAddr() const;

    static string getUniqueLabel(int len);
    int genByType(int type);

    void checkAssignCast(Tree* first, Tree* second);
    void checkCast(Tree* first, Tree* second);
};


#endif //SCANNER_LL_H
