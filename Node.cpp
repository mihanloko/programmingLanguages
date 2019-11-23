//
// Created by mikhail on 14.11.2019.
//

#include "Node.h"

Node::Node() {
    type = ObjFictive;
    lex = "";
    dataType = "";
    size = 0;

}

string Node::toString() {
    return "type: " + to_string(type) + " lex: "  + lex + " dataType: "+ dataType + " size: " + to_string(size);
}
