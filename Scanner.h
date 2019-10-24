//
// Created by mikhail on 01.10.2019.
//

#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H

#include "defs.h"
#include <string>

#define isDigit(c) (c >= '0' && c <= '9')
#define isOctDigit(c) (c >= '0' && c <= '7')
#define isHexDigit(c) (c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F')
#define isLetter(c) (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')

using namespace std;

class Scanner {
public:
    Scanner(string text);
    int scan(string &lexema);
    void setPos(int pos);
    int getPos() const;

    void printError(const string& er);
    void printError(const string& er, string& lex);

private:
    int pos;
    int line;
//    string lex;
    string text;
};


#endif //SCANNER_SCANNER_H
