#ifndef COMPIWET3__SEMANTICS_H_
#define COMPIWET3__SEMANTICS_H_

#include "hw3_output.hpp"
#include "SymbolTable.h"
using namespace std;
class Node;
class symbolTable;
class statement;
class OpenStatement;
class statements;
class call;

vector<symbolTable> globSymTable;
vector<int> offsetStack;

#define YYSTYPE Node*

class Node {
 public:
  string val;
  Node();
  Node(string val);
};

class symbolRow {
    string name;
    int pos;
    vector<string> types;
public:
    symbolRow(string name, int pos, vector<string> types);
};

class symbolTable {
 public:
  bool mainExits = false;
  vector<symbolRow> symbolTable;
};

class program : Node {
 public:
  program();
};

void m_glob() {
  symbolTable new_scope;
  vector < string > print_vec = {"VOID", "STRING"};
  symbolRow print_our("print", 0, print_vec);
  vector < string > printi_vec = {"VOID", "INT"};
  symbolRow printi_our("print", 0, printi_vec);
  new_scope.symbolTable.push_back(print_our);
  new_scope.symbolTable.push_back(printi_our);
  offsetStack.push_back(0);
  globSymTable.push_back(new_scope);
  return;
}

class type : Node {
 public:
    string typeName;
    type(string typeName);
};

class typeAnnotation : Node {
 public:
    string annoType;
    typeAnnotation(string annoType = "");
};

class exp : Node {
 public:
  exp(exp &exp);
  exp(exp &firstExp, string op, exp &secExp);
  exp(string id);
  exp(call &call);
  exp(int val, bool isB = false);
  exp(bool val);
  exp(string op, exp &exp);
  exp(typeAnnotation &typeAnnotation, type &type, exp &exp);
};

class expList : Node {
public:
    expList(exp &exp);
    expList(exp &exp, expList &expList);
};

class call : Node {
public:
    call(string id, expList &expList);
    call(string id);
};

class SimpleStatement : Node {
public:
    SimpleStatement(string cmd); //return, break, continue
    SimpleStatement(statements &statements);
    SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id);
    SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id, exp &exp);
    SimpleStatement(string id, exp &exp);
    SimpleStatement(call &call);
    SimpleStatement(exp &exp);

};

class ClosedStatement : Node {
public:
    ClosedStatement(SimpleStatement &SimpleStatement);
    ClosedStatement(string keyWord, exp &exp, ClosedStatement &ClosedStatement, OpenStatement &OpenStatement);
    ClosedStatement(string keyWord, exp &exp, ClosedStatement &ClosedStatement);
};

class OpenStatement : Node {
public:
    OpenStatement(string keyWord, exp &exp, statement &statement);
    OpenStatement(string keyWord, exp &exp, ClosedStatement &ClosedStatement, OpenStatement &OpenStatement);
    OpenStatement(string keyWord, exp &exp, OpenStatement &OpenStatement);
};

class statement : Node {
public:
    statement(OpenStatement &OpenStatement);
    statement(ClosedStatement &ClosedStatement);
};

class statements : Node {
public:
    statements(statement &statement);
    statements(statements &statements, statement &statement);
};

class formalsDecl : Node {
public:
    formalsDecl(typeAnnotation &typeAnnotation, type &type, string id);
};

class formalsList : Node {
public:
    formalsList(formalsDecl &formalsDecl);
    formalsList(formalsDecl &formalsDecl, formalsList &formalsList);
};

class formals : Node {
public:
    formals();
    formals(formalsList &formalsList);
};

class retType : Node {
public:
    retType(type &type);
};

class funcsDecl : Node {
public:
    funcsDecl(retType &type, string id, formals &formals, statements &statements);
};

class funcs : Node {
public:
    funcs();
};
#endif //COMPIWET3__SEMANTICS_H_
