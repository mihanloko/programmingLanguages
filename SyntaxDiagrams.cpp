//
// Created by mikhail on 18.10.2019.
//

#include "SyntaxDiagrams.h"

SyntaxDiagrams::SyntaxDiagrams(Scanner *scanner) {
    this->scanner = scanner;
}

void SyntaxDiagrams::program() {
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
                    //todo error
                }
                break;
            case END:
                isOk = false;
                break;
            case ERROR:
                //todo printError
                isOk = false;
                break;
        }
    }

}

void SyntaxDiagrams::classDefinition() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != WORD_CLASS) {
        //todo printError
    }
    type = scanner->scan(lex);
    if (type != IDENT) {
        //todo printError
    }
    type = scanner->scan(lex);
    if (type != CURLY_LEFT) {
        //todo printError
    }
    classContent();
    type = scanner->scan(lex);
    if (type != CURLY_RIGHT) {
        //todo printError
    }
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
            //todo printError
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
        //todo printError
    }
    scanner->setPos(pos);
    variableList();
    type = scanner->scan(lex);
    if (type != SEMICOLON) {
        //todo printError
    }

}

void SyntaxDiagrams::variableList() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type != IDENT) {
        //todo error
    }
    scanner->setPos(pos);
    variable();
    pos = scanner->getPos();
    type = scanner->scan(lex);

    while (type != SEMICOLON) {

        if (type == IDENT) {
            scanner->setPos(pos);
            variable();
        } else {
            //todo error
        }
        pos = scanner->getPos();
        type = scanner->scan(lex);
    }
    scanner->setPos(pos);

}

void SyntaxDiagrams::variable() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type != IDENT) {
        //todo error
    }
    if (type == ASSIGN) {
        expression();
    } else if (type == SQUARE_LEFT) {
        type = scanner->scan(lex);
        if (!isConst) {
            //todo error
        }
        type = scanner->scan(lex);
        if (type != SQUARE_RIGHT) {
            //todo error
        }
    } else if (type == SEMICOLON) {
        scanner->setPos(pos);
    } else {
        //todo error
    }
}

void SyntaxDiagrams::main() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != TYPE_INT) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type != WORD_MAIN) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type != ROUND_LEFT) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type != ROUND_RIGHT) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type != CURLY_LEFT) {
        //todo error
    }
    operatorsAndVariables();
    type = scanner->scan(lex);
    if (type != CURLY_RIGHT) {
        //todo error
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
            //todo error
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
    while (type != CURLY_RIGHT) {//todo подумать так ли это
        scanner->setPos(pos);
        oneOperator();
        pos = scanner->getPos();
        type = scanner->scan(lex);
    }
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
        //todo error
    }
    operatorsAndVariables();
    type = scanner->scan(lex);
    if (type != CURLY_RIGHT) {
        //todo error
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
            //todo error
        }
    }
}

void SyntaxDiagrams::operatorIf() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != WORD_IF) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type != ROUND_LEFT) {
        //todo error
    }
    expression();
    type = scanner->scan(lex);
    if (type != ROUND_RIGHT) {
        //todo error
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
        //todo error
    }
    expression();
    type = scanner->scan(lex);
    if (type != SEMICOLON) {
        //todo error
    }
}

void SyntaxDiagrams::expression() {
    // + - ( ident const
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type != ROUND_LEFT && type != PLUS && type != MINUS && type != IDENT && !isConst) {
        //todo error
    }
    if (type == ROUND_LEFT || type == PLUS || type == MINUS || isConst) {
        scanner->setPos(pos);
        a1();
    } else if (type == IDENT) {
        type = scanner->scan(lex);
        if (type == DOT) {
            while (type != ASSIGN) {
                type = scanner->scan(lex);
                if (type != IDENT) {
                    //todo error
                }
                type = scanner->scan(lex);
                if (type != DOT && type != ASSIGN) {
                    scanner->setPos(pos);
                    a1();
                    return;
                }
            }
            scanner->setPos(pos);
            assign();
        } else if (type == SQUARE_LEFT) {
            type = scanner->scan(lex);
            if (type != IDENT || !isConst) {
                //todo error
            }
            type = scanner->scan(lex);
            if (type != SQUARE_RIGHT) {
                //todo error
            }
            type = scanner->scan(lex);
            if (type == ASSIGN) {
                scanner->setPos(pos);
                assign();
            } else {
                scanner->setPos(pos);
                a1();
            }
        } else if (type == ASSIGN) {
            scanner->setPos(pos);
            assign();
        } else {
            scanner->setPos(pos);
            a1();
        }
    } else {
        //todo error
    }
}

void SyntaxDiagrams::a1() {
    a2();
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == EQUALS || type == NOT_EQUAL) {
        while (type == EQUALS || type == NOT_EQUAL) {
            a2();
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
}

void SyntaxDiagrams::a2() {
    a3();
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == GREATER || type == GREATER_EQUAL || type == LESS || type == LESS_EQUAL) {
        while (type == GREATER || type == GREATER_EQUAL || type == LESS || type == LESS_EQUAL) {
            a3();
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
}

void SyntaxDiagrams::a3() {
    a4();
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == LEFT_SHIFT || type == RIGHT_SHIFT) {
        while (type == LEFT_SHIFT || type == RIGHT_SHIFT) {
            a4();
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
}

void SyntaxDiagrams::a4() {
    a5();
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == PLUS || type == MINUS) {
        while (type == PLUS || type == MINUS) {
            a5();
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
}

void SyntaxDiagrams::a5() {
    a6();
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type == STAR || type == SLASH || type == PERCENT) {
        while (type == STAR || type == SLASH || type == PERCENT) {
            a6();
            pos = scanner->getPos();
            type = scanner->scan(lex);
        }
        scanner->setPos(pos);
    } else {
        scanner->setPos(pos);
    }
}

void SyntaxDiagrams::a6() {
    string lex;
    int type;
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    if (type != PLUS && type != MINUS) {
        scanner->setPos(pos);
    }
    a7();
}

void SyntaxDiagrams::a7() {
    string lex;
    int type;
    int pos1 = scanner->getPos();
    type = scanner->scan(lex);
    if (type == ROUND_LEFT) {
        expression();
        type = scanner->scan(lex);
        if (type != ROUND_RIGHT) {
            //todo error
        }
    } else if (type == IDENT) {
        int pos = scanner->getPos();
        type = scanner->scan(lex);
        if (type == SQUARE_LEFT) {
            scanner->setPos(pos1);
            arrayAccess();
        } else if (type == DOT) {
            scanner->setPos(pos1);
            classAccess();
        } else {
            scanner->setPos(pos);
        }
    } else if (isConst) {
        return;
    } else {
        //todo error
    }
}

void SyntaxDiagrams::arrayAccess() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != IDENT) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type != SQUARE_LEFT) {
        //todo error
    }
    type = scanner->scan(lex);
    if (!isConst) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type != SQUARE_RIGHT) {
        //todo error
    }
}

void SyntaxDiagrams::classAccess() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != IDENT) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type != DOT) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type != IDENT) {
        //todo error
    }
    int pos = scanner->getPos();
    type = scanner->scan(lex);
    while (type == DOT) {
        type = scanner->scan(lex);
        if (type != IDENT) {
            //todo error
        }
        pos = scanner->getPos();
        type = scanner->scan(lex);
    }
    scanner->setPos(pos);
}

void SyntaxDiagrams::assign() {
    string lex;
    int type;
    type = scanner->scan(lex);
    if (type != IDENT) {
        //todo error
    }
    type = scanner->scan(lex);
    if (type == ASSIGN) {
        expression();
    } else if (type == SQUARE_LEFT) {
        type = scanner->scan(lex);
        if (!(isConst || type == IDENT)) {
            //todo error
        }
        type = scanner->scan(lex);
        if (type != SQUARE_RIGHT) {
            //todo error
        }
        type = scanner->scan(lex);
        if (type != ASSIGN) {
            //todo error
        } else {
            expression();
        }
    } else if (type == DOT) {
        while (type == DOT) {
            type = scanner->scan(lex);
            if (type != IDENT) {
                //todo error
            }
            type = scanner->scan(lex);
        }
        if (type == ASSIGN) {
            expression();
        } else {
            //todo error
        }
    }
}
