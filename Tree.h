//
// Created by mikhail on 14.11.2019.
//

#ifndef SCANNER_TREE_H
#define SCANNER_TREE_H


#include "Node.h"
#include "Scanner.h"

class Tree {
private:
    Tree *left, *right, *parent;
    Node *node;
public:
    Tree *getLeft() { return left;}

    Node *getNode() const;

    static Scanner* scanner;

    static Tree *cur;

    Tree(Tree *l, Tree *r, Tree *u, Node *Data);

    Tree();

    Tree(Node* data);

    ~Tree();

    void SetLeft(Node *Data);

    void SetRight(Node *Data);

    Tree *FindUp(Tree *From, string id);

    Tree *FindUp(string id);

    Tree *FindRightLeft(Tree *From, string id);

    Tree *FindRightLeft(string id);

    void Print(int k);

    //sem11
    Tree *goUp(Tree* blockParent);

    //sem10, sem2 etc
    Tree *openBlock(string lex, Type type);

    Tree *isExist(string lex);

    Tree *findFiled(Tree *p, string field);

    Tree *checkArray(Tree *p, Tree *e);

    Tree * checkAssignCompatible(Tree * t, Tree * g);

    Tree *findClassDefinition(string lex);

    Tree *createVar(Tree *type, string lex);

    Tree *makeVarArray(string size);

    Tree *makeTypeFromArray(Tree *pTree);

    Tree *makeIntVar();

    Tree* check5Compatible(Tree *t, Tree *g);

    Tree *check4Compatible(Tree *t, Tree *g);

    Tree *check3Compatible(Tree *t, Tree *g);

    Tree *check2Compatible(Tree *t, Tree *g);

    Tree *check1Compatible(Tree *t, Tree *g);

    Tree *FindUpOneLevel(Tree *From, string id);

    int DupControl(Tree *Addr, string a);

    Tree* copy(Tree *p);
    Tree* copy();
};


#endif //SCANNER_TREE_H
