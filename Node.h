//
// Created by mikhail on 14.11.2019.
//

#ifndef SCANNER_NODE_H
#define SCANNER_NODE_H

#include <string>
#include <ostream>

class Tree;

using namespace std;

enum Type {
    ObjVar = 1, // простая переменная
    ObjArray, //массив
    ObjStruct, // класс
    ObjStructDefinition, // тип структуры
    ObjMain, // функция main, должна быть единственной
    ObjFictive,
    ObjUnknown
};

enum TypeName {
    ObjInt = 1,
    ObjChar,
    ObjClass
};

struct Node {
    Type type;  //тип
    string lex;  //обозначение
    TypeName typeName;
    Tree* dataType; // тип переменной или массива //rework
    int size; // размер массива
    string stringTypeName;

    union {
        int* intArray;
        char* charArray;
        int intVariable;
        char charVariable;
        Tree** structArray;
    } data; // значение

    string toString();

    Node();
};


#endif //SCANNER_NODE_H
