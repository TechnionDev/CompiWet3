//
// Created by ZachiLoy on 18/12/2021.
//

#ifndef COMPIWET3_TOKENSCLASSES_H
#define COMPIWET3_TOKENSCLASSES_H

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include "enums.h"

using namespace std;


class Node {
public:
    Node(){}
    virtual ~Node() {}
};

//======================================token's Classes=====================================//

class Type : public Node {
    typeEnum name;
public:
    Type(typeEnum name) : name(name) {}
};

class Op : public Node {
    opEnum type;
public:
    Op(opEnum opType) : type(opType) {}
    Op(char * opType) {
        std::string strType(opType);
        if(strType == "=="){
            type = EQ;
        }
        else if(strType == "!="){
            type = NEQ;
        }
        else if(strType == "<"){
            type = ST;
        }
        else if(strType == ">"){
            type = GT;
        }
        else if(strType == "<="){
            type = STEQ;
        }
        else if(strType == ">="){
            type = GTEQ;
        }
        else if(strType == "-"){
            type = MINUS;
        }
        else if(strType == "+"){
            type = PLUS;
        }
        else if(strType == "*"){
            type = MULT;
        }
        else if(strType == "/"){
            type = DIV;
        }
        else if(strType == "&&"){
            type = AND;
        }
        else if(strType == "||"){
            type = OR;
        }
        else if(strType == "!"){
            type = NOT;
        }
    }
};

class Bool : public Node {
    boolEnum type;
public:
    Bool(boolEnum boolType) : type(boolType) {}
};

class Cmd : public Node {
    cmdEnum type;
public:
    Cmd(cmdEnum cmdType) : type(cmdType)  {}
};

class Parenthesis : public Node {
    parenEnum type;
public:
    Parenthesis(parenEnum parenType) : type(parenType) {}
};

class Annotation : public Node {
    typeAnno type;
public:
    Annotation(typeAnno annoType) : type(annoType){}
};

class Num : public Node {
    int val;
    std::string str;
public:
    Num(std::string strNum) : val(CharPointerToInt(strNum)) , str(strNum) {}
    int getVal() {return val;}
    std::string getStr() {return str;}
};

class Id : public Node {
    std::string IdStr;
public:
    Id(std::string strId) : IdStr(strId) {}
    std::string GetVal() {return IdStr;}
};

class Str : public Node {
    std::string str;
public:
    Str(std::string strVal) : str(strVal) {}
};

#endif //COMPIWET3_TOKENSCLASSES_H
