//
// Created by mikhail on 18.10.2019.
//

#include <iostream>
#include "SyntaxDiagrams.h"

using namespace std;

SyntaxDiagrams::SyntaxDiagrams(Scanner *scanner) {
    this->scanner = scanner;

    Node* node = new Node();
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

bool SyntaxDiagrams::program() {
    bool isOk = true;
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    while (isOk) {
        switch (type) {
            case WORD_CLASS:
                scanner->setPos(pos);
                classDefinition();
                break;
            case TYPE_CHAR:
            case IDENT:
                scanner->setPos(pos);
                variableDefinition();
                break;
            case TYPE_INT:
                type = scanner->scan(lex);
                if (type == WORD_MAIN) {
                    scanner->setPos(pos);
                    main();
                } else if (type == IDENT) {
                    scanner->setPos(pos);
                    variableDefinition();
                } else {
                    scanner->printError("class, main или объявление переменной", lex);
                    exit(0);
                }
                break;
            case END:
                return true;
            case ERROR:
                //scanner->printError(, lex);
                exit(0);
                isOk = false;
                break;
        }
        pos = scanner->getPos();
        type = scanner->scan(lex);
    }

}

void SyntaxDiagrams::classDefinition() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != WORD_CLASS) {
        scanner->printError("class", lex);
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != IDENT) {
        scanner->printError("идентификатор", lex);
        exit(0);
    }
    if (Tree::isFlagInterpret() && Tree::cur->FindUp(lex) != nullptr) {
        scanner->printSemError("Идентификатор " + lex + "уже объявлен", lex.size());
        exit(0);
    }
    Tree *old = nullptr;
    if (Tree::isFlagInterpret())
        old = Tree::cur->openBlock(lex, ObjStructDefinition);
    type = scanner->scan(lex);
    if (type != CURLY_LEFT) {
        scanner->printError("{", lex);
        exit(0);
    }
    classContent();
    type = scanner->scan(lex);
    if (type != CURLY_RIGHT) {
        scanner->printError("}", lex);
        exit(0);
    }
    if (Tree::isFlagInterpret())
        Tree::cur->goUp(old);
}

void SyntaxDiagrams::classContent() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    while (type != CURLY_RIGHT) {

        if (type == TYPE_INT || type == TYPE_CHAR || type == IDENT) {
            scanner->setPos(pos);
            variableDefinition();
        } else {
            scanner->printError("тип данных", lex);
            exit(0);
        }

        pos = scanner->getPos();
        type = scanner->scan(lex);
    }
    scanner->setPos(pos);
}

void SyntaxDiagrams::variableDefinition() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type != TYPE_INT && type != TYPE_CHAR && type != IDENT) {
        scanner->printError("тип данных", lex);
        exit(0);
    }
    Tree* typeDef = nullptr;
    if (Tree::isFlagInterpret())
        typeDef = Tree::cur->findClassDefinition(lex);
    //scanner->setPos(pos);
    variableList(typeDef);
    type = scanner->scan(lex);
    if (type != SEMICOLON) {
        scanner->printError(";", lex);
        exit(0);
    }

}

void SyntaxDiagrams::variableList(Tree *typeDef) {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type != IDENT) {
        scanner->printError("идентификатор", lex);
        exit(0);
    }
    scanner->setPos(pos);
    variable(typeDef);
    pos = scanner->getPos();
    type = scanner->scan(lex);

    while (type != SEMICOLON) {

        if (type == IDENT) {
            scanner->setPos(pos);
            variable(typeDef);
        } else {
            scanner->printError("идентификатор", lex);
            exit(0);
        }
        pos = scanner->getPos();
        type = scanner->scan(lex);
        if (type == VIRGULE) {
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
    }
    scanner->setPos(pos);

}

void SyntaxDiagrams::variable(Tree *typeDef) {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type != IDENT) {
        scanner->printError("идентификатор", lex);
        exit(0);
    }
    Tree *var = nullptr;
    if (Tree::isFlagInterpret())
        var = Tree::cur->createVar(typeDef, lex);
    pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == ASSIGN) {
        Tree *expType = expression();
        if (Tree::isFlagInterpret())
            Tree::cur->checkAssignCompatible(var, expType);
    } else if (type == SQUARE_LEFT) {
        type = scanner->scan(lex);
        if (!isConst) {
            scanner->printError("константа", lex);
            exit(0);
        }
        var->makeVarArray(lex);
        type = scanner->scan(lex);
        if (type != SQUARE_RIGHT) {
            scanner->printError("]", lex);
            exit(0);
        }
    } else if (type == SEMICOLON) {
        scanner->setPos(pos);
    } else if (type == VIRGULE) {
        ;
    } else {
        scanner->printError("\',\' или \';\'", lex);
        exit(0);
    }
}

void SyntaxDiagrams::main() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != TYPE_INT) {
        scanner->printError("int", lex);
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != WORD_MAIN) {
        scanner->printError("main", lex);
        exit(0);
    }
    if (Tree::isFlagInterpret() && Tree::cur->FindUp(lex) != nullptr) {
        scanner->printSemError("Идентификатор " + lex + "уже объявлен", lex.size());
        exit(0);
    }
    Tree *old = nullptr;
    if (Tree::isFlagInterpret())
        old = Tree::cur->openBlock(lex, ObjMain);
    type = scanner->scan(lex);
    if (type != ROUND_LEFT) {
        scanner->printError("(", lex);
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != ROUND_RIGHT) {
        scanner->printError(")", lex);
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != CURLY_LEFT) {
        scanner->printError("{", lex);
        exit(0);
    }
    operatorsAndVariables();
    type = scanner->scan(lex);
    if (type != CURLY_RIGHT) {
        scanner->printError("}", lex);
        exit(0);
    }
    if (Tree::isFlagInterpret()) {
        Tree::cur->goUp(old);
        delete old->getRight();
    }
}

void SyntaxDiagrams::operatorsAndVariables() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    while (type != CURLY_RIGHT) {
        if (type == TYPE_INT || type == TYPE_CHAR) {
            scanner->setPos(pos);
            variableDefinition();
        } else if (type == SEMICOLON || type == CURLY_LEFT || type == WORD_IF || type == WORD_RETURN ||
                   type == ROUND_LEFT || isConst) {
            scanner->setPos(pos);
            operators();
        } else if (type == IDENT) {
            type = scanner->scan(lex);
            if (type == IDENT) {
                scanner->setPos(pos);
                variableDefinition();
            } else {
                scanner->setPos(pos);
                operators();
            }
        } else {
            scanner->printError("оператор или объявление переменной", lex);
            exit(0);
        }
        pos = scanner->getPos();
        type = scanner->scan(lex);
    }
    scanner->setPos(pos);
}

void SyntaxDiagrams::operators() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    //while (type != CURLY_RIGHT) {//todo подумать так ли это
        scanner->setPos(pos);
        oneOperator();
        pos = scanner->getPos();
        type = scanner->scan(lex);
    //}
    scanner->setPos(pos);
}

void SyntaxDiagrams::oneOperator() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == SEMICOLON) {
        return;
    } else if (type == CURLY_LEFT) {
        scanner->setPos(pos);
        compoundOperator();
    } else {
        scanner->setPos(pos);
        simpleOperator();
    }
}

void SyntaxDiagrams::compoundOperator() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != CURLY_LEFT) {
        scanner->printError("{", lex);
        exit(0);
    }
    Tree *old = nullptr;
    if (Tree::isFlagInterpret())
        old = Tree::cur->openBlock("", ObjFictive);
    operatorsAndVariables();
    type = scanner->scan(lex);
    if (type != CURLY_RIGHT) {
        scanner->printError("}", lex);
        exit(0);
    }
    if (Tree::isFlagInterpret()) {
        Tree::cur->goUp(old);
        delete old->getRight();
    }
}

void SyntaxDiagrams::simpleOperator() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == WORD_IF) {
        scanner->setPos(pos);
        operatorIf();
    } else if (type == WORD_RETURN) {
        scanner->setPos(pos);
        operatorReturn();
    } else {
        scanner->setPos(pos);
        expression();
        type = scanner->scan(lex);
        if (type != SEMICOLON) {
            scanner->printError(";", lex);
            exit(0);
        }
    }
}

void SyntaxDiagrams::operatorIf() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != WORD_IF) {
        scanner->printError("if", lex);
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != ROUND_LEFT) {
        scanner->printError("(", lex);
        exit(0);
    }
    expression();
    type = scanner->scan(lex);
    if (type != ROUND_RIGHT) {
        scanner->printError(")", lex);
        exit(0);
    }
    oneOperator();
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == WORD_ELSE) {
        oneOperator();
    } else {
        scanner->setPos(pos);
    }
}

void SyntaxDiagrams::operatorReturn() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != WORD_RETURN) {
        scanner->printError("return", lex);
        exit(0);
    }
    Tree* expType = expression();
    if (expType->getNode()->type != ObjVar) {
        scanner->printSemError("Должно быть int или char", 0);
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != SEMICOLON) {
        scanner->printError(";", lex);
        exit(0);
    }
}

Tree* SyntaxDiagrams::expression() {
    // + - ( ident const
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type != ROUND_LEFT && type != PLUS && type != MINUS && type != IDENT && !isConst) {
        scanner->printError("выражение", lex);
        exit(0);
    }
    if (type == ROUND_LEFT || type == PLUS || type == MINUS || isConst) {
        scanner->setPos(pos);
        return a1();
    } else if (type == IDENT) {
        if (Tree::isFlagInterpret())
            Tree::cur->isExist(lex);
        type = scanner->scan(lex);
        if (type == DOT) {
            while (type != ASSIGN) {
                type = scanner->scan(lex);
                if (type != IDENT) {
                    scanner->printError("идентификатор", lex);
                    exit(0);
                }
                type = scanner->scan(lex);
                if (type != DOT && type != ASSIGN) {
                    scanner->setPos(pos);
                    return a1();
                }
            }
            scanner->setPos(pos);
            return assign();
        } else if (type == SQUARE_LEFT) {
            type = scanner->scan(lex);
            if (type != IDENT && !isConst) {
                scanner->printError("константа или идентификатор", lex);
                exit(0);
            }
            type = scanner->scan(lex);
            if (type != SQUARE_RIGHT) {
                scanner->printError("]", lex);
                exit(0);
            }
            type = scanner->scan(lex);
            if (type == ASSIGN) {
                scanner->setPos(pos);
                return assign();
            } else {
                scanner->setPos(pos);
                return a1();
            }
        } else if (type == ASSIGN) {
            scanner->setPos(pos);
            return assign();
        } else {
            scanner->setPos(pos);
            return a1();
        }
    } else {
        scanner->printError("выражение", lex);
        exit(0);
    }
}

Tree *SyntaxDiagrams::a1() {
    Tree *t = a2();
    Tree *res = t;
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == EQUALS || type == NOT_EQUAL) {
        while (type == EQUALS || type == NOT_EQUAL) {
            Tree *g = a2();
            if (Tree::isFlagInterpret())
                res = Tree::cur->check1Compatible(t, g);
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
    return res;
}

Tree* SyntaxDiagrams::a2() {
    Tree *t = a3();
    Tree *res = t;
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == GREATER || type == GREATER_EQUAL || type == LESS || type == LESS_EQUAL) {
        while (type == GREATER || type == GREATER_EQUAL || type == LESS || type == LESS_EQUAL) {
            Tree *g = a3();
            if (Tree::isFlagInterpret())
                res = Tree::cur->check2Compatible(t, g);
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
    return res;
}

Tree* SyntaxDiagrams::a3() {
    Tree *t = a4();
    Tree *res = t;
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == LEFT_SHIFT || type == RIGHT_SHIFT) {
        while (type == LEFT_SHIFT || type == RIGHT_SHIFT) {
            Tree *g = a4();
            if (Tree::isFlagInterpret())
                res = Tree::cur->check3Compatible(t, g);
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
    return res;
}

Tree* SyntaxDiagrams::a4() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    bool flag = false;
    if (type == PLUS || type == MINUS) {
        flag = true;
    } else {
        scanner->setPos(pos);
    }

    Tree *t = a5();
    Tree *res = t;

    if (flag) {
        if (t->getNode()->type == ObjVar || t->getNode()->type == ObjUnknown) {
            ;
        }
        else {
            scanner->printSemError("Невозможный тип операции для унарной операции + или -", 0);
            exit(0);
        }
    }

    pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == PLUS || type == MINUS) {
        while (type == PLUS || type == MINUS) {
            Tree *g = a5();
            if (Tree::isFlagInterpret())
                res = Tree::cur->check4Compatible(t, g);
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
    return res;
}

Tree* SyntaxDiagrams::a5() {
    Tree *res;
    Tree *t = a6();
    res = t;
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == STAR || type == SLASH || type == PERCENT) {
        while (type == STAR || type == SLASH || type == PERCENT) {
            Tree *g = a6();
            if (Tree::isFlagInterpret())
                res = Tree::cur->check5Compatible(t, g);
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
    return res;
}

Tree* SyntaxDiagrams::a6() {
    /*string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type != PLUS && type != MINUS) {
        scanner->setPos(pos);
    }
    Tree::cur->Print(0);
    Tree *expType = a7();
    if (expType->getNode()->type == ObjInt || expType->getNode()->type == ObjChar || expType->getNode()->type == ObjUnknown) {
        return expType;
    }
    else {
        scanner->printSemError("Невозможный тип операции для унарной операции + или -", 0);
        exit(0);
    }*/
    return a7();
}

Tree* SyntaxDiagrams::a7() {
    string lex;
    int type;
    int pos1 = scanner->getPos();
    type = scanner->scan(lex);
    string oldLex = lex;
    if (type == ROUND_LEFT) {
        Tree *typeExp = expression();
        type = scanner->scan(lex);
        if (type != ROUND_RIGHT) {
            scanner->printError(")", lex);
            exit(0);
        }
        return typeExp;
    } else if (type == IDENT) {
        int pos = scanner->getPos();
        type = scanner->scan(lex);
        if (type == SQUARE_LEFT) {
            scanner->setPos(pos1);
            return arrayAccess();
        } else if (type == DOT) {
            scanner->setPos(pos1);
            return classAccess();
        } else {
            scanner->setPos(pos);
            return Tree::cur->isExist(oldLex);
        }
    } else if (isConst) {
        return Tree::cur->makeIntVar();
    } else {
        scanner->printError("элементарное выражение", lex);
        exit(0);
    }
}

Tree* SyntaxDiagrams::arrayAccess() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != IDENT) {
        scanner->printError("идентификатор", lex);
        exit(0);
    }
    Tree *var = Tree::cur->isExist(lex);
    if (Tree::isFlagInterpret() && var->getNode()->type != ObjArray && var->getNode()->type != ObjUnknown) {
        scanner->printSemError(lex + " не является массивом", lex.size());
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != SQUARE_LEFT) {
        scanner->printError("[", lex);
        exit(0);
    }
    type = scanner->scan(lex);
    if (!isConst) {
        scanner->printError("константа", lex);
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != SQUARE_RIGHT) {
        scanner->printError("]", lex);
        exit(0);
    }
    return Tree::cur->makeTypeFromArray(var);
}

Tree* SyntaxDiagrams::classAccess() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != IDENT) {
        scanner->printError("идентификатор", lex);
        exit(0);
    }
    Tree* var = Tree::cur->FindUp(lex);
    if (Tree::isFlagInterpret() && var == nullptr) {
        scanner->printSemError("Идентификатор " + lex + " не объявлен", lex.size());
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != DOT) {
        scanner->printError("точка", lex);
        exit(0);
    }
    type = scanner->scan(lex);
    if (type != IDENT) {
        scanner->printError("идентификатор", lex);
        exit(0);
    }
    var = Tree::cur->findFiled(var, lex);
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    while (type == DOT) {
        type = scanner->scan(lex);
        if (type != IDENT) {
            scanner->printError("идентификатор", lex);
            exit(0);
        }
        var = Tree::cur->findFiled(var, lex);
        pos = scanner->getPos();
        type = scanner->scan(lex);
    }
    scanner->setPos(pos);
    return var;
}

Tree *SyntaxDiagrams::assign() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != IDENT) {
        scanner->printError("идентификатор", lex);
        exit(0);
    }
    Tree *n = Tree::cur->isExist(lex);
    if (Tree::isFlagInterpret() && n == nullptr) {
        scanner->printSemError("Идентификатор " + lex + " не объявлен", lex.size());
        exit(0);
    }
    type = scanner->scan(lex);
    Tree *expType;
    if (type == ASSIGN) {
        expType = expression();
        return Tree::cur->checkAssignCompatible(n, expType);
    } else if (type == SQUARE_LEFT) {
        Tree * indexType;
        /*type = scanner->scan(lex);
        if (!(isConst || type == IDENT)) {
            scanner->printError("константа или идентификатор", lex);
            exit(0);
        }*/
        indexType = expression();
        Tree * arrayType = Tree::cur->checkArray(n, indexType);
        type = scanner->scan(lex);
        if (type != SQUARE_RIGHT) {
            scanner->printError("]", lex);
            exit(0);
        }
        type = scanner->scan(lex);
        if (type != ASSIGN) {
            scanner->printError("=", lex);
            exit(0);
        } else {
            expType = expression();
            return Tree::cur->checkAssignCompatible(arrayType, expType);
        }
    } else if (type == DOT) {
        while (type == DOT) {
            type = scanner->scan(lex);
            if (type != IDENT) {
                scanner->printError("идентификатор", lex);
                exit(0);
            }
            n = Tree::cur->findFiled(n, lex);
            type = scanner->scan(lex);
        }
        if (type == ASSIGN) {
            expType = expression();
            return Tree::cur->checkAssignCompatible(n, expType);
        } else {
            scanner->printError("=", lex);
            exit(0);
        }
    }
}
