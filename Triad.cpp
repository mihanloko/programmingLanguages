//
// Created by mikhail on 22.09.2021.
//

#include "Triad.h"

Operand::Operand(int triadNum) {
    this->type = ADDRESS;
    this->value.address = triadNum;
}

Operand::Operand(Node *operand) {
    this->type = NODE;
    this->value.node = operand;
}

Operand::Operand(string operand) : Operand(new Node(operand)) {}

Triad::Triad(int operation, Operand *op1, Operand *op2) : Triad(operation) {
    this->operand1 = op1;
    this->operand2 = op2;
}

Triad::Triad(int operation) {
    this->operation = operation;
}

int Triad::getOperation() const {
    return operation;
}

Operand *Triad::getOperand1() const {
    return operand1;
}

Operand *Triad::getOperand2() const {
    return operand2;
}

void Triad::setOperand1(Operand *operand1) {
    Triad::operand1 = operand1;
}

void Triad::setOperand2(Operand *operand2) {
    Triad::operand2 = operand2;
}


