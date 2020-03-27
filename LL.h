//
// Created by mikhail on 14.12.2019.
//

#ifndef SCANNER_LL_H
#define SCANNER_LL_H


#include "Scanner.h"
#include "defs.h"
#include <string>
#include <map>

using namespace std;

class LL {
private:
    Scanner *scanner;
    int magazine[5000];
    int pos = 0;
    map<int, string> words;
    string lastLex;
public:
    LL(Scanner *scanner, map<int, string> &words);
    void analyze();
};


#endif //SCANNER_LL_H
