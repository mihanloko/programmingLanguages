//
// Created by mikhail on 01.10.2019.
//

#include <iostream>
#include "Scanner.h"

using namespace std;

Scanner::Scanner(string text) {
    this->text = text;
    this->pos = 0;
    this->line = 0;
}

int Scanner::scan(string &lex) {

    lex.clear();
    start:
    while (text[pos] == ' ' || text[pos] == '\t' || text[pos] == '\n') {
        if (text[pos] == '\n') line++;
        pos++;
    }
    if (text[pos] == '\0') return END;

    if (text[pos] == '/' && text.size() > pos + 1 && text[pos + 1] == '/') {
        while (text[pos] != '\n' && text[pos] != '\0') {
            if (text[pos] == '\n') line++;
            pos++;
        }
        goto start;
    }


    //digit
    if (isDigit(text[pos])) {
        //10 c/c
        if (text[pos] != '0') {
            lex.push_back(text[pos++]);
            while (isDigit(text[pos]))
                lex.push_back(text[pos++]);
            if (lex.size() > 10) {
                printError("Слишком длинная константа");
                return ERROR;
            }
            return DEC;
        } else {
            //16 c/c
            lex.push_back(text[pos++]);
            if (text[pos] == 'x') {
                lex.push_back(text[pos++]);
                bool digit = false;
                while (isHexDigit(text[pos])) {
                    lex.push_back(text[pos++]);
                    digit = true;
                }
                if (digit) {
                    if (lex.size() > 10) {
                        printError("Слишком длинная константа");
                        return ERROR;
                    }
                    return HEX;
                }
                else {
                    printError("нет цифр после 0x в шестнадцатиричной константе");
                    pos++;
                    return ERROR;
                }
            }
                //8 c/c
            else {
                while (isOctDigit(text[pos]))
                    lex.push_back(text[pos++]);
                if (isDigit(text[pos])) {
                    printError("десятичный символ в восьмиричной константе");
                    pos++;
                    return ERROR;
                }
                if (lex.size() > 10) {
                    printError("Слишком длинная константа");
                    return ERROR;
                }
                return OCT;
            }
        }
    }
        //char
    else if (isLetter(text[pos])) {
        lex.push_back(text[pos++]);
        while (isLetter(text[pos]) || isDigit(text[pos]))
            lex.push_back(text[pos++]);
        if (lex == "int") {
            return TYPE_INT;
        } else if (lex == "char") {
            return TYPE_CHAR;
        } else if (lex == "class") {
            return WORD_CLASS;
        } else if (lex == "if") {
            return WORD_IF;
        } else if (lex == "else") {
            return WORD_ELSE;
        } else if (lex == "return") {
            return WORD_RETURN;
        }
        else if (lex == "main") {
            return WORD_MAIN;
        } else {
            if (lex.size() > 80) {
                printError("Слишком длинный идентификатор");
                return ERROR;
            }
            return IDENT;
        }
    }
        //символы
    else {
        switch (text[pos]) {
            case '=':
                lex.push_back(text[pos++]);
                if (text[pos] == '=') {
                    lex.push_back(text[pos++]);
                    return EQUALS;
                } else {
                    return ASSIGN;
                }
            case '>':
                lex.push_back(text[pos++]);
                switch (text[pos]) {
                    case '>':
                        lex.push_back(text[pos++]);
                        return RIGHT_SHIFT;
                    case '=':
                        lex.push_back(text[pos++]);
                        return GREATER_EQUAL;
                    default:
                        return GREATER;
                }
            case '<':
                lex.push_back(text[pos++]);
                switch (text[pos]) {
                    case '<':
                        lex.push_back(text[pos++]);
                        return LEFT_SHIFT;
                    case '=':
                        lex.push_back(text[pos++]);
                        return LESS_EQUAL;
                    default:
                        return LESS;
                }
            case '.':
                lex.push_back(text[pos++]);
                return DOT;
            case ',':
                lex.push_back(text[pos++]);
                return VIRGULE;
            case ';':
                lex.push_back(text[pos++]);
                return SEMICOLON;
            case '*':
                lex.push_back(text[pos++]);
                return STAR;
            case '/':
                lex.push_back(text[pos++]);
                return SLASH;
            case '%':
                lex.push_back(text[pos++]);
                return PERCENT;
            case '+':
                lex.push_back(text[pos++]);
                return PLUS;
            case '-':
                lex.push_back(text[pos++]);
                return MINUS;
            case '(':
                lex.push_back(text[pos++]);
                return ROUND_LEFT;
            case ')':
                lex.push_back(text[pos++]);
                return ROUND_RIGHT;
            case '[':
                lex.push_back(text[pos++]);
                return SQUARE_LEFT;
            case ']':
                lex.push_back(text[pos++]);
                return SQUARE_RIGHT;
            case '{':
                lex.push_back(text[pos++]);
                return CURLY_LEFT;
            case '}':
                lex.push_back(text[pos++]);
                return CURLY_RIGHT;
            case '!':
                lex.push_back(text[pos++]);
                if (text[pos] == '=') {
                    lex.push_back(text[pos++]);
                    return NOT_EQUAL;
                } else {
                    printError("Должно было быть =");
                    pos++;
                    return ERROR;
                }
            default:
                printError("Неизвестный символ");
                pos++;
                return ERROR;
        }
    }

//    return ERROR;
}

void Scanner::printError(const string& er) {
    cout << "Ошибка " << er << " строка " << line << " позиция " << pos << " символ " << text[pos] << endl;
}

int Scanner::getPos() const {
    return pos;
}

void Scanner::setPos(int pos) {
    Scanner::pos = pos;
}

void Scanner::printError(const string &er, string &lex) {
    cout << "Ожидалось " << er << ", получено " << lex << " позиция " << pos;
}
