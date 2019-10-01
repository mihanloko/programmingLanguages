#include <fstream>
#include <iostream>
#include "Scanner.h"

using namespace std;

string getData(const string& fileName) {
    string sf;
    ifstream inf;

    inf.open(fileName);
    getline(inf, sf, '\0');
    inf.close();
    return sf;
}

int main () {
    string text = getData("input.txt");

    Scanner *scanner = new Scanner(text);
    int code;
    string lex;
    while ((code = scanner->scan(lex))) {
        cout << code << " " << lex;
    }


    return 0;
}