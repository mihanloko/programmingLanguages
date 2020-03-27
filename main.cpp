#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "Scanner.h"
//#include "SyntaxDiagrams.h"
#include "LL.h"

using namespace std;

string getData(const string& fileName) {
    string sf;
    ifstream inf;

    inf.open(fileName);
    getline(inf, sf, '\0');
    inf.close();
    return sf;
}

map<int, string> getMap() {
    ifstream in("const.txt");

    map<int, string> words;
    while (!in.eof()) {
        string str;
        int n;
        in >> str >> n;
        words.insert(make_pair(n, str));
    }

    in.close();

    return words;
}

int main (int argc, char *argv[]) {
    string text = getData(argv[1]);

    map<int, string> words = getMap();

    Scanner *scanner = new Scanner(text);
    int code;
    string lex;
    bool errors = false;
    code = scanner->scan(lex);
    while (code != END) {
        if (code == ERROR) errors = true;
        //cout << words[code] << " " << lex << endl;
        code = scanner->scan(lex);
    }
    //cout << words[code] << " " << lex << endl;
    scanner->setPos(0);
    /*if (!errors || errors) {
        SyntaxDiagrams *diagram = new SyntaxDiagrams(scanner);
        bool result = diagram->program();
        if (result) cout << "Ошибок нет";
    }*/
    /*LL *ll = new LL(scanner, words);
    ll->analyze();*/

    return 0;
}