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
