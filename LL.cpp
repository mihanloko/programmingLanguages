//
// Created by mikhail on 14.12.2019.
//

#include <iostream>
#include "LL.h"
#include "Tree.h"

LL::LL(Scanner *scanner, map<int, string> &words) {
    this->scanner = scanner;
    this->words = words;

    // включение семантического дерева
    Node *node = new Node();
    node->lex = "int";
    node->type = ObjStructDefinition;
    Tree *currentNode = new Tree(node);
    node = new Node();
    node->lex = "char";
    node->type = ObjStructDefinition;
    currentNode->SetLeft(node);
    Tree::cur = currentNode->getLeft();
    Tree::scanner = scanner;
}

void LL::analyze() {
    magazine[pos] = S;
    int run = 1;
    int type;
    string lex;
    while (run) {

        if (magazine[pos] < S) { ///terminal
            type = scanner->scan(lex);
            if (type == magazine[pos]) {
                if (type == END) run = 0;
                else {
                    //type = scanner->scan(lex);
                    pos--;
                }
            } else {
                scanner->printError(words[magazine[pos]], lex);
                cout << endl;
//                for (int i = pos + 5; i > -1; i--)
//                    cout << words[magazine[i]] << endl;
                return;
            }
        } else if (magazine[pos] >= SET_CLASS) { ///semantic function
            switch (magazine[pos]) {
                case BACK:
                    break;
                case START_MAIN:
                    break;
                case SAVE_TYPE:
                    break;
                case CHECK_TYPE:
                    break;
                case CHECK_CONDITION:
                    break;
                case PUSH:
                    break;
                case MATCH_ASSIGN:
                    break;
                case MATCH_1:
                    break;
                case MATCH_2:
                    break;
                case MATCH_3:
                    break;
                case MATCH_4:
                    break;
                case MATCH_5:
                    break;
                case FIND:
                    break;
                case MAKE_ARRAY:
                    break;
                case POP:
                    break;
                case CHECK_ARRAY:
                    break;
                case ADD:
                    break;
                case NEW_BLOCK:
                    break;
                case MATCH_6:
                    break;
                case MATCH_CONST:
                    break;
                case FIND_FIELD:
                    break;
            }
        }
        else { ///not terminal
            switch (magazine[pos]) {
                case S: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == END)
                        magazine[pos++] = END;
                    else {
                        magazine[pos++] = S;
                        magazine[pos++] = DES;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case DES: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    switch (type) {
                        case IDENT:
                        case TYPE_CHAR:
                            magazine[pos++] = VARDEF;
                            break;
                        case WORD_CLASS:
                            magazine[pos++] = CL;
                            break;
                        case TYPE_INT:
                            string saveLex = lex;
                            int savePos = scanner->getPos();
                            type = scanner->scan(lex);
                            if (type == WORD_MAIN) {
                                magazine[pos++] = MAIN;
                            }
                            else {
                                magazine[pos++] = VARDEF;
                            }
                            scanner->setPos(savePos);
                            lex = saveLex;
                            break;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case CL:
                    magazine[pos++] = SEMICOLON;
                    magazine[pos++] = CURLY_RIGHT;
                    magazine[pos++] = CLIN;
                    magazine[pos++] = CURLY_LEFT;
                    magazine[pos++] = SET_CLASS;
                    magazine[pos++] = IDENT;
                    magazine[pos++] = WORD_CLASS;
                    break;
                case CLIN:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == IDENT || type == TYPE_INT || type == TYPE_CHAR) {
                        magazine[pos++] = CLIN;
                        magazine[pos++] = VARDEF;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    //todo maybe else
                    break;
                case MAIN:
                    magazine[pos++] = CURLY_RIGHT;
                    magazine[pos++] = OPVAR;
                    magazine[pos++] = CURLY_LEFT;
                    magazine[pos++] = START_MAIN;
                    magazine[pos++] = ROUND_RIGHT;
                    magazine[pos++] = ROUND_LEFT;
                    magazine[pos++] = WORD_MAIN;
                    magazine[pos++] = TYPE_INT;
                    break;
                case OPVAR:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    switch (type) {
                        case DEC:
                        case CONST:
                        case HEX:
                        case OCT:
                        case WORD_IF:
                        case SEMICOLON:
                        case CURLY_LEFT:
                        case ROUND_LEFT:
                        case PLUS:
                        case MINUS:
                            magazine[pos++] = OPVAR;
                            magazine[pos++] = OP;
                            break;
                        case TYPE_INT:
                        case TYPE_CHAR:
                            magazine[pos++] = OPVAR;
                            magazine[pos++] = VARDEF;
                            break;
                        case IDENT:
                            string saveLex = lex;
                            int savePos = scanner->getPos();
                            type = scanner->scan(lex);
                            if (type == IDENT) {
                                magazine[pos++] = OPVAR;
                                magazine[pos++] = VARDEF;
                            }
                            else {
                                magazine[pos++] = OPVAR;
                                magazine[pos++] = OP;
                            }
                            scanner->setPos(savePos);
                            lex = saveLex;
                            break;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case T: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    switch (type) {
                        case TYPE_INT:
                            magazine[pos++] = SAVE_TYPE;
                            magazine[pos++] = TYPE_INT;
                            break;
                        case TYPE_CHAR:
                            magazine[pos++] = SAVE_TYPE;
                            magazine[pos++] = TYPE_CHAR;
                            break;
                        case IDENT:
                            magazine[pos++] = SAVE_TYPE;
                            magazine[pos++] = CHECK_TYPE;
                            magazine[pos++] = IDENT;
                            break;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case OP: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == SEMICOLON)
                        magazine[pos++] = EMOP;
                    else if (type == CURLY_LEFT)
                        magazine[pos++] = COMPOP;
                    else
                        magazine[pos++] = SIMOP;
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case EMOP:
                    magazine[pos++] = SEMICOLON;
                    break;
                case COMPOP:
                    magazine[pos++] = BACK;
                    magazine[pos++] = CURLY_RIGHT;
                    magazine[pos++] = OPVAR;
                    magazine[pos++] = CURLY_LEFT;
                    magazine[pos++] = NEW_BLOCK;
                    break;
                case SIMOP: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == WORD_IF)
                        magazine[pos++] = IF;
                    else {
                        magazine[pos++] = SEMICOLON;
                        magazine[pos++] = EXP;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case IF:
                    magazine[pos++] = ELSE;
                    magazine[pos++] = OP;
                    magazine[pos++] = CHECK_CONDITION;
                    magazine[pos++] = ROUND_RIGHT;
                    magazine[pos++] = EXP;
                    magazine[pos++] = ROUND_LEFT;
                    magazine[pos++] = WORD_IF;
                    break;
                case ELSE: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == WORD_ELSE) {
                        magazine[pos++] = OP;
                        magazine[pos++] = WORD_ELSE;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case ASSIGN:
                    magazine[pos++] = MATCH_ASSIGN;
                    magazine[pos++] = A1;
                    magazine[pos++] = ASSIGN;
                    magazine[pos++] = OBJ;
                    break;
                case A0:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == IDENT) {

                        while (type == DOT || type == IDENT || type == SQUARE_LEFT || type == SQUARE_RIGHT ||
                               type == DEC || type == OCT || type == HEX || type == CONST) {
                            type = scanner->scan(lex);
                        }

                        if (type == ASSIGN) {
                            magazine[pos++] = MATCH_ASSIGN;
                            magazine[pos++] = A1;
                            magazine[pos++] = ASSIGN;
                            magazine[pos++] = OBJ;
                        }
                        else {
                            magazine[pos++] = A1;
                        }
                        lex = saveLex;
                        scanner->setPos(savePos);
                    }
                    else {
                        magazine[pos++] = A1;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A1:
                    magazine[pos++] = A11;
                    magazine[pos++] = A2;
                    break;
                case A2:
                    magazine[pos++] = A22;
                    magazine[pos++] = A3;
                    break;
                case A3:
                    magazine[pos++] = A33;
                    magazine[pos++] = A4;
                    break;
                case A4:
                    magazine[pos++] = A44;
                    magazine[pos++] = A5;
                    break;
                case A5:
                    magazine[pos++] = A55;
                    magazine[pos++] = A6;
                    break;
                case A11:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == EQUALS || type == NOT_EQUAL) {
                        magazine[pos++] = MATCH_1;
                        magazine[pos++] = A1;
                        magazine[pos++] = type;
                    }
                    /*else {
                        magazine[pos++] = A2;
                    }*/
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A22:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == GREATER || type == GREATER_EQUAL || type == LESS || type == LESS_EQUAL) {
                        magazine[pos++] = MATCH_2;
                        magazine[pos++] = A2;
                        magazine[pos++] = type;
                    }
                    /*else {
                        magazine[pos++] = A3;
                    }*/
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A33:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == LEFT_SHIFT || type == RIGHT_SHIFT) {
                        magazine[pos++] = MATCH_3;
                        magazine[pos++] = A3;
                        magazine[pos++] = type;
                    }
                    /*else {
                        magazine[pos++] = A4;
                    }*/
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A44:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == PLUS || type == MINUS) {
                        magazine[pos++] = MATCH_4;
                        magazine[pos++] = A4;
                        magazine[pos++] = type;
                    }
                    /*else {
                        magazine[pos++] = A5;
                    }*/
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A55:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == STAR || type == SLASH || type == PERCENT) {
                        magazine[pos++] = MATCH_5;
                        magazine[pos++] = A5;
                        magazine[pos++] = type;
                    }
                    /*else {
                        magazine[pos++] = A6;
                    }*/
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A6:
                    magazine[pos++] = A7;
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == PLUS || type == MINUS) {
                        magazine[pos++] = type;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A7:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    switch (type) {
                        case IDENT:
                            magazine[pos++] = OBJ;
                            break;
                        case DEC:
                        case OCT:
                        case HEX:
                        case CONST:
                            magazine[pos++] = PUSH;
                            magazine[pos++] = type;
                            break;
                        case ROUND_LEFT:
                            magazine[pos++] = ROUND_RIGHT;
                            magazine[pos++] = A1;
                            magazine[pos++] = ROUND_LEFT;
                            break;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case EXP:
                    magazine[pos++] = A0;
                    break;
                case VARDEF:
                    magazine[pos++] = SEMICOLON;
                    magazine[pos++] = VARLIST;
                    magazine[pos++] = T;
                    break;
                case VARLIST:
                    magazine[pos++] = VARDOP;
                    magazine[pos++] = VAR;
                    break;
                case VARDOP:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == VIRGULE) {
                        magazine[pos++] = VARDOP;
                        magazine[pos++] = VAR;
                        magazine[pos++] = VIRGULE;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case VAR:
                    magazine[pos++] = P;
                    magazine[pos++] = SQ;
                    magazine[pos++] = ADD;
                    magazine[pos++] = FIND;
                    magazine[pos++] = IDENT;
                    break;
                case P: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == ASSIGN) {
                        magazine[pos++] = MATCH_ASSIGN;
                        magazine[pos++] = A1;
                        magazine[pos++] = ASSIGN;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case OBJ:
                    magazine[pos++] = ARCL;
                    magazine[pos++] = PUSH;
                    magazine[pos++] = FIND;
                    magazine[pos++] = IDENT;
                    break;
                case ARCL:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == DOT || type == SQUARE_LEFT) {
                        magazine[pos++] = ARCL;
                        magazine[pos++] = FIELD;
                        magazine[pos++] = AR;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case AR:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == SQUARE_LEFT) {
                        magazine[pos++] = CHECK_ARRAY;
                        magazine[pos++] = SQUARE_RIGHT;
                        magazine[pos++] = MATCH_CONST;
                        magazine[pos++] = EXP;
                        magazine[pos++] = SQUARE_LEFT;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case FIELD:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == DOT) {
                        magazine[pos++] = PUSH;
                        magazine[pos++] = FIND_FIELD;
                        magazine[pos++] = IDENT;
                        magazine[pos++] = DOT;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case SQ:
                {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == SQUARE_LEFT) {
                        magazine[pos++] = MAKE_ARRAY;
                        magazine[pos++] = SQUARE_RIGHT;
                        magazine[pos++] = CONST;
                        magazine[pos++] = SQUARE_LEFT;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
            }
            pos--;
        }

    }
    cout << "Ошибок нет";
}
