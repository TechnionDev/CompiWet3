//
// Created by ZachiLoy on 18/12/2021.
//

#ifndef COMPIWET3_SYMBOLTABLE_H
#define COMPIWET3_SYMBOLTABLE_H

#include "string"
#include "enums.h"
#include <stack>
#include <list>

class Symbol{
    std::string name;
    typeEnum type;
    int offset;
public:
    Symbol(std::string name, typeEnum type, int offset) : name(name), type(type), offset(offset){};
    std::string getName(){ return  name;};
    typeEnum getType(){ return type;};
    int getOffset(){ return offset;};
};

class SymbolTable {
    std::stack<std::list<Symbol*>> *symtbl;
    bool mainExists;
public:
    SymbolTable() : symtbl( new std::stack<std::list<Symbol*>>()), mainExists(false){};
    bool isExist(Symbol* symbol);
    void addSymbol(std::string name, typeEnum type, int offset);
    std::list<Symbol*> getCurrentScope();
    void addScope();
    void removeScope();
    bool isMAinExist();
};


#endif //COMPIWET3_SYMBOLTABLE_H
