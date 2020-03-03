//
// Created by mikhail on 14.11.2019.
//

#include "Node.h"

Node::Node() {
    type = ObjFictive;
    lex = "";
    stringTypeName = "";
    size = 0;

}

string Node::toString() {
    return "type: " + to_string(type) + " lex: "  + lex + " dataType: "+ stringTypeName + " size: " + to_string(size);
}

Node *Node::copy() {
    Node* newNode = new Node();
    newNode->stringTypeName = stringTypeName;
    newNode->typeName = typeName;
    newNode->type = type;
    newNode->dataType = dataType;
    newNode->data = data;
    newNode->lex = lex;
    newNode->size = size;

    return newNode;
}

Node::~Node() {
    if (type == ObjArray) {
        switch (typeName) {
            case ObjInt:
                delete data.intArray;
                break;
            case ObjChar:
                delete data.charArray;
                break;
            case ObjClass:
                for (int i = 0; i < size; i++)
                    delete[] data.structArray;
                delete data.structArray;
                break;
        }
    }
}


