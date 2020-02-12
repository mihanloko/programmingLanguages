//
// Created by mikhail on 14.11.2019.
//

#include <iostream>
#include "Tree.h"

using namespace std;

Tree *Tree::cur = (Tree *) nullptr;
Scanner *Tree::scanner = (Scanner *) nullptr;

Tree::Tree(Tree *l, Tree *r, Tree *u, Node *Data) {
    left = l;
    right = r;
    parent = u;
    node = Data;
}

Tree::Tree() {
    node = new Node();
    left = right = parent = nullptr;
}

void Tree::SetLeft(Node *Data) {
    left = new Tree(nullptr, nullptr, this, Data);
}

Tree::~Tree() {
    delete left;
    delete right;
    delete node;
}

void Tree::SetRight(Node *Data) {
    right = new Tree(nullptr, nullptr, this, Data);
}

Tree *Tree::FindUp(Tree *From, string id) {
    Tree *cur = From;
    while (cur != nullptr && (cur->node->lex != id)) {
        cur = cur->parent;
    }
    return cur;
}

Tree *Tree::FindUpOneLevel(Tree *From, string id)
// Поиск элемента id вверх по дереву от текущей вершины From.
// Поиск осуществляется на одном уровне вложенности по левым связям
{
    Tree *i = From;
// текущая вершина поиска
    while ((i != NULL) && (i->parent != nullptr && i->parent->right != i)) {
        if (id == i->node->lex)
            return i; // нaшли совпадающий идентификатор
        i = i->parent;
// поднимаемся наверх по связям
    }
    return NULL;
}

int Tree::DupControl(Tree *Addr, string a)
// Проверка идентификатора а на повторное описание внутри блока.
// Поиск осуществляется вверх от вершины Addr.
{
    if (FindUpOneLevel(Addr, a) == NULL) return 0;
    return 1;
}

Tree *Tree::FindUp(string id) {
    return FindUp(this, id);
}

Tree *Tree::FindRightLeft(string id) {
    return FindRightLeft(this, id);
}

Tree *Tree::FindRightLeft(Tree *From, string id) {
    Tree *cur = From->right;
    while (cur != nullptr && (cur->node->lex != id)) {
        cur = cur->left;
    }
    return cur;
}

Tree::Tree(Node *data) {
    this->node = data;
    left = right = parent = nullptr;
}

Tree *Tree::goUp(Tree *blockParent) {
    while (cur != blockParent)
        cur = cur->parent;
    return cur;
}

Tree *Tree::openBlock(string lex, Type type) {
    if (!lex.empty() && cur->FindUp(lex) != nullptr) {
        scanner->printSemError("Идентификатор уже объявлен " + lex, lex.size());
    }
    Node *newNode = new Node();
    newNode->lex = lex;
    newNode->type = type;
    cur->SetLeft(newNode);
    cur = cur->left;
    Tree *res = cur;
    Node *fictive = new Node();
    fictive->type = ObjFictive;
    cur->SetRight(fictive);
    cur = cur->right;
    return res;
}

void Tree::Print(int k) {
    for (int i = 0; i < k; ++i) {
        printf("\t");
    }
    printf("Вершина с данными %s ----->", node->lex.c_str());
    if (left != nullptr) printf("  слева данные %s", left->node->lex.c_str());
    if (right != nullptr) printf("  справа данные %s", right->node->lex.c_str());
    printf("\n");
    if (left != nullptr) left->Print(k);
    if (right != nullptr) right->Print(k + 1);
}

Tree *Tree::isExist(string lex) {
    Tree *n = cur->FindUp(lex);
    if (n == nullptr) {
        scanner->printSemError("Идентификатор " + lex + " не объявлен", lex.size());
        Node *fakeNode = new Node();
        fakeNode->lex = lex;
        fakeNode->type = ObjUnknown;
        cur->SetLeft(fakeNode);
        cur = cur->left;
        return cur;
    }
    return n;
}

Tree *Tree::findFiled(Tree *p, string field) {
    if (p->node->type != ObjStruct) {
        scanner->printSemError("Попытка обратиться к полю, но " + p->node->lex + " не является структурой",
                               field.size() + 1);
        exit(0);
    }

    Tree *typeDefinition = p->node->dataType;
    string type = typeDefinition->getNode()->stringTypeName;
//    while (typeDefinition != nullptr && typeDefinition->node->type != ObjStructDefinition)
//        typeDefinition = Tree::cur->FindUp(typeDefinition->parent, type);
    if (typeDefinition == nullptr) {
        scanner->printSemError("Класс не объявлен " + type, type.size());
        exit(0);
    }
    Tree *fieldNode = typeDefinition->FindRightLeft(field);
    if (fieldNode == nullptr) {
        scanner->printSemError("Класс не содержит поле " + field, field.size() + 1);
        Tree *s = typeDefinition->right;
        while (s->left != nullptr) {
            s = s->left;
        }
        Node *fakeNode = new Node();
        fakeNode->lex = field;
        fakeNode->type = ObjUnknown;
        s->SetLeft(fakeNode);
        fieldNode = s->left;
        exit(0);
    }

    return fieldNode;
}

Tree *Tree::checkArray(Tree *p, Tree *e) {
    if (p->node->type != ObjArray) {
        scanner->printSemError(p->node->lex + " не является массивом, оператор [] неуместен", 1);
        exit(0);
    }

    if (e->node->type != ObjVar) {
        scanner->printSemError("Значеник выражения не является целочисленным типом", 0);
        exit(0);
    }

    return p->getNode()->dataType;
}

Node *Tree::getNode() const {
    return node;
}

Tree *Tree::checkAssignCompatible(Tree *t, Tree *g) {
    if (t->node->type == ObjStructDefinition || g->node->type == ObjStructDefinition ||
        t->node->type == ObjMain || g->node->type == ObjMain ||
        t->node->type == ObjArray || g->node->type == ObjArray) {
        scanner->printSemError("Неприсваемый тип", 0);
        exit(0);
    }

    if (t->node->type == ObjStruct || g->node->type == ObjStruct) {
        if (t->node->type == ObjStruct && g->node->type == ObjStruct) {
            if (t->node->dataType == g->node->dataType)
                return t->node->dataType;
            else {
                scanner->printSemError("разные классы", 0);
                exit(0);
            }
        }
        else {
            scanner->printSemError("разные классы", 0);
            exit(0);
        }
    }

    if (t->node->type == ObjUnknown || g->node->type == ObjUnknown)
        return t->node->type == ObjUnknown ? t : g;

    if (t->node->typeName == ObjChar && g->node->typeName == ObjChar)
        return t;
    if (t->node->typeName == ObjInt || g->node->typeName == ObjInt)
        return t->node->typeName == ObjInt ? t : g;


    scanner->printSemError("Неверный тип", 0);
    exit(0);
}

Tree *Tree::findClassDefinition(string lex) {
    Tree *type = cur->FindUp(lex);
    while (type != nullptr && type->node->type != ObjStructDefinition)
        type = type->FindUp(type->parent, lex);
    if (type == nullptr) {
        scanner->printSemError("класс " + lex + " не объявлен", 0);
        exit(0);
    }
    if (type->node->type != ObjStructDefinition) {
        scanner->printSemError(lex + " не является классом", 0);
        exit(0);
    }
    return type;
}

Tree *Tree::createVar(Tree *type, string lex) {

    Tree *p = cur->FindUp(lex);
    if (DupControl(this, lex)) {
        scanner->printSemError("Объект " + lex + " уже существует", lex.size());
        exit(0);
    }
    Node *n = new Node();
    n->dataType = type;
    n->stringTypeName = type->node->lex;
    n->lex = lex;
    if (type->node->lex == "int") {
        n->type = ObjVar;
        n->typeName = ObjInt;
    }
    else if (type->node->lex == "char") {
        n->type = ObjVar;
        n->typeName = ObjChar;
    }
    else {
        n->type = ObjStruct;
        n->typeName = ObjClass;
    }

    cur->SetLeft(n);
    cur = cur->left;
    Tree *i = cur;
//    while (i->parent != nullptr)
//        i = i->parent;
    //i->Print(0);
    return cur;
}

Tree *Tree::makeVarArray(string size) {
    this->node->size = stoi(size);
    this->node->type = ObjArray;
    return this;
}

Tree *Tree::makeTypeFromArray(Tree *pTree) {
    Node *n = new Node();
    if (pTree->node->stringTypeName == "int") {
        n->type = ObjVar;
        n->typeName = ObjInt;
    } else if (pTree->node->stringTypeName == "char") {
        n->type = ObjVar;
        n->typeName = ObjClass;
    } else {
        n->type = ObjStruct;
        n->typeName = ObjClass;
    }
    n->dataType = pTree->node->dataType;

    return new Tree(n);
}

Tree *Tree::makeIntVar() {
    Node *n = new Node();
    n->type = ObjVar;
    n->typeName = ObjInt;
    n->stringTypeName = "int";

    return new Tree(n);
}

Tree *Tree::check5Compatible(Tree *t, Tree *g) {
    if (t->node->type == ObjStructDefinition || g->node->type == ObjStructDefinition ||
        t->node->type == ObjMain || g->node->type == ObjMain ||
        t->node->type == ObjArray || g->node->type == ObjArray ||
        t->node->type == ObjStruct || g->node->type == ObjStruct) {
        scanner->printSemError("Невозможный тип для операций *, / или %", 0);
        exit(0);
    }
    if (t->node->type == ObjUnknown || g->node->type == ObjUnknown)
        return t->node->type == ObjUnknown ? t : g;
    if (t->node->typeName == ObjInt || g->node->typeName == ObjInt)
        return t->node->typeName == ObjInt ? t : g;
    if (t->node->typeName == ObjChar && g->node->typeName == ObjChar)
        return t;
    return nullptr;
}

Tree *Tree::check4Compatible(Tree *t, Tree *g) {
    if (t->node->type == ObjStructDefinition || g->node->type == ObjStructDefinition ||
        t->node->type == ObjMain || g->node->type == ObjMain ||
        t->node->type == ObjArray || g->node->type == ObjArray ||
        t->node->type == ObjStruct || g->node->type == ObjStruct) {
        scanner->printSemError("Невозможный тип для операций + или -", 0);
        exit(0);
    }
    if (t->node->type == ObjUnknown || g->node->type == ObjUnknown)
        return t->node->type == ObjUnknown ? t : g;
    if (t->node->typeName == ObjInt || g->node->typeName == ObjInt)
        return t->node->typeName == ObjInt ? t : g;
    if (t->node->typeName == ObjChar && g->node->typeName == ObjChar)
        return t;
    return nullptr;
}

Tree *Tree::check3Compatible(Tree *t, Tree *g) {
    if (t->node->type == ObjStructDefinition || g->node->type == ObjStructDefinition ||
        t->node->type == ObjMain || g->node->type == ObjMain ||
        t->node->type == ObjArray || g->node->type == ObjArray ||
        t->node->type == ObjStruct || g->node->type == ObjStruct) {
        scanner->printSemError("Невозможный тип для операций логического сдвига", 0);
        exit(0);
    }

    if (t->node->type == ObjUnknown || g->node->type == ObjUnknown)
        return t->node->type == ObjUnknown ? t : g;
    if (t->node->typeName == ObjInt || g->node->typeName == ObjInt)
        return t->node->typeName == ObjInt ? t : g;
    if (t->node->typeName == ObjChar && g->node->typeName == ObjChar)
        return t;
    return nullptr;
}

Tree *Tree::check2Compatible(Tree *t, Tree *g) {
    if (t->node->type == ObjStructDefinition || g->node->type == ObjStructDefinition ||
        t->node->type == ObjMain || g->node->type == ObjMain ||
        t->node->type == ObjArray || g->node->type == ObjArray ||
        t->node->type == ObjStruct || g->node->type == ObjStruct) {
        scanner->printSemError("Невозможный тип для операций сравнения", 0);
        exit(0);
    }

    if (t->node->type == ObjUnknown || g->node->type == ObjUnknown)
        return t->node->type == ObjUnknown ? t : g;
    if (t->node->typeName == ObjInt || g->node->typeName == ObjInt)
        return t->node->typeName == ObjInt ? t : g;
    if (t->node->typeName == ObjChar && g->node->typeName == ObjChar)
        return t;
    return nullptr;
}

Tree *Tree::check1Compatible(Tree *t, Tree *g) {
    if (t->node->type == ObjStructDefinition || g->node->type == ObjStructDefinition) {
        scanner->printSemError("Невозможный тип для операций сравнения", 0);
        exit(0);
    }

    if (t->node->type == ObjMain || g->node->type == ObjMain) {
        if (t->node->type == ObjMain && g->node->type == ObjMain)
            return t;
        else {
            scanner->printSemError("Невозможный тип для операций сравнения", 0);
            exit(0);
        }
    }

    if (t->node->type == ObjArray || g->node->type == ObjArray) {
        if (t->node->type == ObjArray && g->node->type == ObjArray) {
            if (t->node->dataType == g->node->dataType)
                return t;
            else {
                scanner->printSemError("Разные типы массивов для сравнения", 0);
                exit(0);
            }
        }
    }

    if (t->node->type == ObjStruct || g->node->type == ObjStruct) {
        if (t->node->type == ObjStruct && g->node->type == ObjStruct) {
            if (t->node->dataType == g->node->dataType)
                return t;
            else {
                scanner->printSemError("Разные типы структур для сравнения", 0);
                exit(0);
            }
        } else {
            scanner->printSemError("Невозможный тип для операций сравнения", 0);
            exit(0);
        }
    }

    if (t->node->type == ObjUnknown || g->node->type == ObjUnknown)
        return t->node->type == ObjUnknown ? t : g;
    if (t->node->typeName == ObjInt || g->node->typeName == ObjInt)
        return t->node->typeName == ObjInt ? t : g;
    if (t->node->typeName == ObjChar && g->node->typeName == ObjChar)
        return t;
    return nullptr;
}

Tree *Tree::copy(Tree *p) {
    Tree* newTree = this->copy();
    newTree->parent= p;
    return newTree;
}

Tree *Tree::copy() {
    Tree* newTree = new Tree();
    newTree->node = node->copy();
    newTree->parent = parent;
    newTree->left = left == nullptr ? nullptr : left->copy();
    newTree->right = right == nullptr ? nullptr : right->copy();
    return newTree;
}
