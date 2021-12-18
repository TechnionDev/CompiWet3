//
// Created by ZachiLoy on 18/12/2021.
//

#include "SymbolTable.h"

//TODO: implement this function
bool SymbolTable::isExist(Symbol *symbol) {
    bool isFound = false;
    return false;
}

void SymbolTable::addSymbol(std::string name, typeEnum type, int offset) {
    this->symtbl->top().push_back(new Symbol(name,type,offset));
}

std::list<Symbol*> SymbolTable::getCurrentScope() {
    return this->symtbl->top();
}

void SymbolTable::addScope() {
    this->symtbl->push(std::list<Symbol*>());
}

void SymbolTable::removeScope() {
    std::list<Symbol*>::iterator it = symtbl->top().begin();
    for(it; it!=symtbl->top().end(); it++){
        delete(*it);
    }
    symtbl->pop();
}

bool SymbolTable::isMAinExist() {
    return this->mainExists;
}
