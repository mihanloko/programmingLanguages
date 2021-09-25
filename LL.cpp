//
// Created by mikhail on 14.12.2019.
//

#include <iostream>
#include "LL.h"


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

        if (magazine[pos] >= 0 && magazine[pos] < S) { ///terminal
            type = scanner->scan(lex);
            if (type == magazine[pos]) {
                if (type == END) run = 0;
                else {
                    //type = scanner->scan(lex);
                    if (type == TYPE_INT || type == TYPE_CHAR || type == IDENT) {
                        lastLex = lex;
                    }
                    pos--;
                }
            } else {
                scanner->printError(words[magazine[pos]], lex);
                cout << endl;
                return;
            }
        } else if (magazine[pos] >= SET_CLASS && magazine[pos] <= PUSH_CONST) { ///semantic function
            switch (magazine[pos]) {
                case BACK:
                    Tree::cur->goUp();
                    break;
                case START_MAIN:
                    Tree::cur->openBlock("main", ObjMain);
                    break;
                case SAVE_TYPE:
                    magazineTypes[typePos++] = Tree::cur->findClassDefinition(lastLex);
                    break;
                case CHECK_TYPE:
                    Tree::cur->findClassDefinition(lastLex);
                    break;
                case PUSH:
                    magazineTypes[typePos++] = Tree::cur->FindUp(lastLex);
                    break;
                case PUSH_CONST:
                    magazineTypes[typePos++] = Tree::cur->makeIntVar();
                    break;
                case MATCH_ASSIGN:
                    magazineTypes[typePos - 1] = Tree::cur->checkAssignCompatible(magazineTypes[typePos - 1],
                                                                                  magazineTypes[typePos - 2]);
                    checkAssignCast(magazineTypes[typePos - 1], magazineTypes[typePos - 2]);
                    typePos--;
                    break;
                case MATCH_1:
                    magazineTypes[typePos - 1] = Tree::cur->check1Compatible(magazineTypes[typePos - 1],
                                                                             magazineTypes[typePos - 2]);
                    checkCast(magazineTypes[typePos - 2], magazineTypes[typePos - 1]);
                    typePos--;
                    break;
                case MATCH_2:
                    magazineTypes[typePos - 1] = Tree::cur->check2Compatible(magazineTypes[typePos - 1],
                                                                             magazineTypes[typePos - 2]);
                    checkCast(magazineTypes[typePos - 2], magazineTypes[typePos - 1]);
                    typePos--;
                    break;
                case MATCH_3:
                    magazineTypes[typePos - 1] = Tree::cur->check3Compatible(magazineTypes[typePos - 1],
                                                                             magazineTypes[typePos - 2]);
                    checkCast(magazineTypes[typePos - 2], magazineTypes[typePos - 1]);
                    typePos--;
                    break;
                case MATCH_4:
                    magazineTypes[typePos - 1] = Tree::cur->check4Compatible(magazineTypes[typePos - 1],
                                                                             magazineTypes[typePos - 2]);
                    checkCast(magazineTypes[typePos - 2], magazineTypes[typePos - 1]);
                    typePos--;
                    break;
                case MATCH_5:
                    magazineTypes[typePos - 1] = Tree::cur->check5Compatible(magazineTypes[typePos - 1],
                                                                             magazineTypes[typePos - 2]);
                    checkCast(magazineTypes[typePos - 2], magazineTypes[typePos - 1]);
                    typePos--;
                    break;
                case MATCH_6:
                    magazineTypes[typePos - 1] = Tree::cur->check6Compatible(magazineTypes[typePos - 1]);
                    break;
                case MATCH_CONST:
                    if (magazineTypes[typePos - 1]->getNode()->typeName != ObjChar &&
                        magazineTypes[typePos - 1]->getNode()->typeName != ObjInt)
                        scanner->printSemError("Значение выражения не явлется целым типом", lastLex.size());
                    break;
                case FIND:
                    lastType = Tree::cur->FindUp(lastLex);
                    if (lastType == nullptr) {
                        scanner->printSemError("Идентификатор " + lastLex + " не объявлен", lastLex.size());
                    }
                    break;
                case MAKE_ARRAY:
                    lastType->makeVarArray();
                    break;
                case POP:
                    typePos--;
                    break;
                case CHECK_ARRAY: {
                    if (lastType->getNode()->type != ObjArray)
                        scanner->printSemError("Объект " + lastType->getNode()->lex + " не является массивом", lastLex.size());
                }
                    break;
                case ADD:
                    lastType = Tree::cur->createVar(magazineTypes[typePos - 1], lastLex);
                    break;
                case NEW_BLOCK:
                    Tree::cur->openBlock("", ObjFictive);
                    break;
                case FIND_FIELD:
                    lastType = lastType->findFiled(lastType, lastLex);
                    break;
                case SET_CLASS:
                    if (Tree::cur->FindUp(lastLex) != nullptr) {
                        scanner->printSemError("Идентификатор " + lastLex + "уже объявлен", lastLex.size());
                        exit(0);
                    }
                    Tree::cur->openBlock(lastLex, ObjStructDefinition);
                    break;
            }
            pos--;
        } else if (magazine[pos] < 0) { /// gen
            switch (magazine[pos]) {
                case GEN_PUSH:
                    operands.push_back(new Operand(new Node(lex)));
                    break;
                case GEN_CMP: {
                    generateArithmeticTriad(TRI_CMP);
                    break;
                }
                case GEN_MUL: {
                    generateArithmeticTriad(TRI_MUL);
                    break;
                }

                case GEN_DIV: {
                    generateArithmeticTriad(TRI_DIV);
                    break;
                }

                case GEN_MOD: {
                    generateArithmeticTriad(TRI_MOD);
                    break;
                }

                case GEN_PLUS: {
                    generateArithmeticTriad(TRI_PLUS);
                    break;
                }

                case GEN_MINUS: {
                    generateArithmeticTriad(TRI_MINUS);
                    break;
                }

                case GEN_ASSIGNMENT: {
                    generateArithmeticTriad(TRI_ASSIGNMENT);
                    break;
                }
                case GEN_EQ: {
                    generateArithmeticTriad(TRI_EQ);
                    break;
                }
                case GEN_NEQ : {
                    generateArithmeticTriad(TRI_NEQ);
                    break;
                }
                case GEN_GT: {
                    generateArithmeticTriad(TRI_GT);
                    break;
                }
                case GEN_GE : {
                    generateArithmeticTriad(TRI_GE);
                    break;
                }
                case GEN_LT: {
                    generateArithmeticTriad(TRI_LT);
                    break;
                }
                case GEN_LE: {
                    generateArithmeticTriad(TRI_LE);
                    break;
                }
                case GEN_LEFT_SHIFT: {
                    generateArithmeticTriad(TRI_LEFT_SHIFT);
                    break;
                }
                case GEN_RIGHT_SHIFT: {
                    generateArithmeticTriad(TRI_RIGHT_SHIFT);
                    break;
                }
                case GEN_IDX: {
                    generateArithmeticTriad(TRI_IDX);
                    break;
                }
                case GEN_DOT: {
                    generateArithmeticTriad(TRI_DOT);
                    break;
                }
                case GEN_IF: {
                    Triad *ifTriad = new Triad(TRI_IF, nullptr, nullptr);
                    triads.push_back(ifTriad);
                    ifData.emplace_back();
                    ifData.back().ifOperand = ifTriad;
                    ifData.back().falseAddress = -1;
                    ifData.back().trueAddress = -1;
                    ifData.back().nopOperand = -1;
                    break;
                }
                case GEN_FORM_IF: {
                    ifData.back().ifOperand->setOperand1(new Operand(ifData.back().trueAddress));
                    ifData.back().ifOperand->setOperand2(new Operand(ifData.back().falseAddress == -1 ? ifData.back().nopOperand : ifData.back().falseAddress));
                    ifData.back().jmpTriad->getOperand1()->value.address = ifData.back().nopOperand;
                    ifData.pop_back();
                    break;
                }
                case GEN_GOTO: {
                    triads.push_back(new Triad(TRI_JMP, new Operand(ifData.back().nopOperand), nullptr));
                    ifData.back().jmpTriad = triads.back();
                    break;
                }
                case GEN_SET_ADDR_NOP: {
                    triads.push_back(new Triad(TRI_NOP));
                    ifData.back().nopOperand = triads.size() - 1;
                    break;
                }
                case GEN_SET_TRUE_ADDR: {
                    ifData.back().trueAddress = triads.size();
                    break;
                }
                case GEN_SET_FALSE_ADDR: {
                    ifData.back().falseAddress = triads.size();
                    break;
                }
                case GEN_PROC: {
                    triads.push_back(new Triad(TRI_PROC, new Operand("main"), nullptr));
                    break;
                }
                case GEN_ENDP: {
                    triads.push_back(new Triad(TRI_ENDP, new Operand("main"), nullptr));
                    break;
                }
            }
            pos--;
        } else { ///not terminal
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
                            } else {
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
                case CLIN: {
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
                    break;
                case MAIN:
                    magazine[pos++] = GEN_ENDP;
                    magazine[pos++] = CURLY_RIGHT;
                    magazine[pos++] = OPVAR;
                    magazine[pos++] = CURLY_LEFT;
                    magazine[pos++] = START_MAIN;
                    magazine[pos++] = ROUND_RIGHT;
                    magazine[pos++] = ROUND_LEFT;
                    magazine[pos++] = GEN_PROC;
                    magazine[pos++] = WORD_MAIN;
                    magazine[pos++] = TYPE_INT;
                    break;
                case OPVAR: {
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
                            } else {
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
                    magazine[pos++] = GEN_GOTO;
                    magazine[pos++] = OP;
                    magazine[pos++] = CHECK_CONDITION;
                    magazine[pos++] = GEN_SET_TRUE_ADDR;
                    magazine[pos++] = GEN_IF;
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
                        magazine[pos++] = GEN_FORM_IF;
                        magazine[pos++] = GEN_SET_ADDR_NOP;
                        magazine[pos++] = OP;
                        magazine[pos++] = GEN_SET_FALSE_ADDR;
                        magazine[pos++] = WORD_ELSE;
                    } else {
                        magazine[pos++] = GEN_FORM_IF;
                        magazine[pos++] = GEN_SET_ADDR_NOP;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case ASSIGN:
                    magazine[pos++] = GEN_ASSIGNMENT;
                    magazine[pos++] = MATCH_ASSIGN;
                    magazine[pos++] = A1;
                    magazine[pos++] = ASSIGN;
                    magazine[pos++] = GEN_PUSH;
                    magazine[pos++] = OBJ;
                    break;
                case A0: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == IDENT) {

                        while (type == DOT || type == IDENT || type == SQUARE_LEFT || type == SQUARE_RIGHT ||
                               type == DEC || type == OCT || type == HEX || type == CONST) {
                            type = scanner->scan(lex);
                        }

                        if (type == ASSIGN) {
                            magazine[pos++] = GEN_ASSIGNMENT;
                            magazine[pos++] = MATCH_ASSIGN;
                            magazine[pos++] = A1;
                            magazine[pos++] = ASSIGN;
                            magazine[pos++] = OBJ;
                        } else {
                            magazine[pos++] = A1;
                        }
                        lex = saveLex;
                        scanner->setPos(savePos);
                    } else {
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
                case A11: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == EQUALS || type == NOT_EQUAL) {
                        magazine[pos++] = genByType(type);
                        magazine[pos++] = MATCH_1;
                        magazine[pos++] = A1;
                        magazine[pos++] = type;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A22: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == GREATER || type == GREATER_EQUAL || type == LESS || type == LESS_EQUAL) {
                        magazine[pos++] = genByType(type);
                        magazine[pos++] = MATCH_2;
                        magazine[pos++] = A2;
                        magazine[pos++] = type;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A33: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == LEFT_SHIFT || type == RIGHT_SHIFT) {
                        magazine[pos++] = genByType(type);
                        magazine[pos++] = MATCH_3;
                        magazine[pos++] = A3;
                        magazine[pos++] = type;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A44: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == PLUS || type == MINUS) {
                        magazine[pos++] = genByType(type);
                        magazine[pos++] = MATCH_4;
                        magazine[pos++] = A4;
                        magazine[pos++] = type;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case A55: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == STAR || type == SLASH || type == PERCENT) {
                        magazine[pos++] = genByType(type);
                        magazine[pos++] = MATCH_5;
                        magazine[pos++] = A5;
                        magazine[pos++] = type;
                    }
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
                case A7: {
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
                            magazine[pos++] = GEN_PUSH;
                            magazine[pos++] = PUSH_CONST;
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
                case VARDOP: {
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
                    magazine[pos++] = GEN_PUSH;
                    magazine[pos++] = IDENT;
                    break;
                case ARCL: {
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
                case AR: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == SQUARE_LEFT) {
                        magazine[pos++] = SQUARE_RIGHT;
                        magazine[pos++] = MATCH_CONST;
                        magazine[pos++] = GEN_IDX;
                        magazine[pos++] = EXP;
                        magazine[pos++] = CHECK_ARRAY;
                        magazine[pos++] = SQUARE_LEFT;
                        magazine[pos++] = GEN_PUSH;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case FIELD: {
                    string saveLex = lex;
                    int savePos = scanner->getPos();
                    type = scanner->scan(lex);
                    if (type == DOT) {
                        magazine[pos++] = GEN_DOT;
                        magazine[pos++] = PUSH;
                        magazine[pos++] = FIND_FIELD;
                        magazine[pos++] = GEN_PUSH;
                        magazine[pos++] = IDENT;
                        magazine[pos++] = DOT;
                        magazine[pos++] = GEN_PUSH;
                    }
                    scanner->setPos(savePos);
                    lex = saveLex;
                }
                    break;
                case SQ: {
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

void LL::outOperands() {
    cout << "\nОперанды: ";
    for (int i = 0; i < operands.size(); i++) {
        outOneOperand(operands[i]);
        cout << " ";
    }
    cout << "\n";
}

void LL::outOneOperand(Operand *operand) {
    if (operand == nullptr || operand->type == EMPTY)
        cout << "-";
    else if (operand->type == ADDRESS)
        cout << "(" << operand->value.address << ")";
    else if (operand->type == NODE)
        cout << operand->value.node->lex;
}

void LL::generateArithmeticTriad(int operation) {
    Operand *operand2 = getOperand();
    Operand *operand1 = getOperand();
    triads.push_back(new Triad(operation, operand1, operand2));
    operands.push_back(new Operand(getLastTriadAddr()));
}

Operand *LL::getOperand() {
    return getTopValue(operands, "operands");
}

template<typename TYPE>
TYPE LL::getTopValue(vector<TYPE> &st, const string &name) {
    if (st.empty())
        invalid_argument("Empty collection " + name);
    auto res = st.back();
    st.pop_back();
    return res;
}

int LL::getLastTriadAddr() const {
    return triads.size() - 1;
}

int LL::genByType(int type) {
    switch (type) {
        case STAR:
            return GEN_MUL;
        case SLASH:
            return GEN_DIV;
        case PERCENT:
            return GEN_MOD;
        case GREATER:
            return GEN_GT;
        case GREATER_EQUAL:
            return GEN_GE;
        case LESS:
            return GEN_LT;
        case LESS_EQUAL:
            return GEN_LE;
        case EQUALS:
            return GEN_EQ;
        case NOT_EQUAL:
            return GEN_NEQ;
        case LEFT_SHIFT:
            return GEN_LEFT_SHIFT;
        case RIGHT_SHIFT:
            return GEN_RIGHT_SHIFT;
        case PLUS:
            return GEN_PLUS;
        case MINUS:
            return GEN_MINUS;
        default:
            return -1;
    }
}

void LL::outTriads() {
    cout << "Триады: " << endl;
    for (int i = 0; i < triads.size(); i++) {
        cout << i << ") ";
        outOneTriad(triads[i]);
        cout << endl;
    }
}

void LL::outOneTriad(Triad *triad) {
    cout << codeOperationToString(triad->getOperation()) << " ";
    outOneOperand(triad->getOperand1());
    cout << " ";
    outOneOperand(triad->getOperand2());
}


string LL::codeOperationToString(int code) {
    string str;

    switch (code) {
        case TRI_MUL:
            str = "*";
            break;
        case TRI_DIV:
            str = "/";
            break;
        case TRI_MOD:
            str = "%";
            break;
        case TRI_PLUS:
            str = "+";
            break;
        case TRI_MINUS:
            str = "-";
            break;
        case TRI_ASSIGNMENT:
            str = "=";
            break;
        case TRI_GT:
            str = ">";
            break;
        case TRI_LT:
            str = "<";
            break;
        case TRI_GE:
            str = ">=";
            break;
        case TRI_LE:
            str = "<=";
            break;
        case TRI_EQ:
            str = "==";
            break;
        case TRI_NEQ:
            str = "!=";
            break;
        case TRI_CMP:
            str = "cmp";
            break;
        case TRI_CALL:
            str = "call";
            break;
        case TRI_PROC:
            str = "proc";
            break;
        case TRI_ENDP:
            str = "endp";
            break;
        case TRI_JMP:
            str = "jmp";
            break;
        case TRI_RET:
            str = "ret";
            break;
        case TRI_MOV:
            str = "mov";
            break;
        case TRI_NOP:
            str = "nop";
            break;
        case TRI_IF:
            str = "if";
            break;
        case TRI_LEFT_SHIFT:
            str = "<<";
            break;
        case TRI_RIGHT_SHIFT:
            str = ">>";
            break;
        case TRI_DOT:
            str = ".";
            break;
        case TRI_IDX:
            str = "idx";
            break;
        case TRI_UNIQUE_LABEL:
            str = getUniqueLabel(4) + ':';
            break;
        case TRI_INT_CHAR:
            str = "i->ch";
            break;
        case TRI_CHAR_INT:
            str = "ch->i";
            break;
        default:
            throw invalid_argument("invalid operation code: " + to_string(code));
    }

    return str;
}

string LL::getUniqueLabel(int len) {
    {
        string tmp_s;
        static const char alphanum[] =
                "0123456789"
                "abcdefghijklmnopqrstuvwxyz";

        tmp_s.reserve(len);

        for (int i = 0; i < len; ++i)
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


        return tmp_s;
    }
}

void LL::checkAssignCast(Tree *first, Tree *second) {
    if (first->getNode()->typeName != second->getNode()->typeName) {
        if (second->getNode()->typeName == ObjChar && first->getNode()->typeName == ObjInt) {
            triads.push_back(new Triad(TRI_INT_CHAR, operands.back(), nullptr));
            operands.pop_back();
            operands.push_back(new Operand(getLastTriadAddr()));
        } else if (second->getNode()->typeName == ObjInt && first->getNode()->typeName == ObjChar) {
            triads.push_back(new Triad(TRI_CHAR_INT, operands.back(), nullptr));
            operands.pop_back();
            operands.push_back(new Operand(getLastTriadAddr()));
        }
    }
}

void LL::checkCast(Tree *first, Tree *second) {
    if (first->getNode()->typeName != second->getNode()->typeName) {
        if (first->getNode()->typeName == ObjChar) {
            triads.push_back(new Triad(TRI_CHAR_INT, operands.back(), nullptr));
            operands.pop_back();
            operands.push_back(new Operand(getLastTriadAddr()));
        } else if (second->getNode()->typeName == ObjChar) {
            triads.push_back(new Triad(TRI_CHAR_INT, operands.back(), nullptr));
            operands.pop_back();
            operands.push_back(new Operand(getLastTriadAddr()));
        }
    }
}
