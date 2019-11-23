//
// Created by mikhail on 14.11.2019.
//

#ifndef SCANNER_NODE_H
#define SCANNER_NODE_H

#include <string>
#include <ostream>

using namespace std;

enum Type {
    ObjInt = 1, // простая переменная int
    ObjChar, // простая переменная char
    ObjArray, //массив
    ObjStruct, // класс
    ObjStructDefinition, // тип структуры
    ObjMain, // функция main, должна быть единственной
    ObjUnknown, // неопределнный тип
    ObjFictive // фиктивный узел
};

struct Node {
    Type type; // тип
    string lex; // обозначение
    string dataType; // тип переменной или массива
    int size; // размер массива
    Node();

    string toString();
};


#endif //SCANNER_NODE_H
