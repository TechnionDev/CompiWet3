#ifndef COMPIWET3__SEMANTICS_H_
#define COMPIWET3__SEMANTICS_H_

#include "hw3_output.hpp"
using namespace std;
class Node;

#define YYSTYPE Node*

class Node {
 public:
  string val;
  Node();
  Node(string val);
};

class symbolRow {
  symbolRow(string name, int pos, vector<string> types);
  string name;
  int pos;
  string types;
};

class symbolTable {
 public:
  vector<symbolRow> symbolTable;
};

class program : Node {
 public:
  program();
};

class funcs : Node {
 public:
  funcs();
};

class funcsDecl : Node {
 public:
  funcsDecl(retType &type, string id, formals &formals, statements &statements);
};

class retType : Node {
 public:
  retType(type &type);
};

class formals : Node {
 public:
  formals();
  formals(formalsList &formalsList);
};

class formalsList : Node {
 public:
  formalsList(formalsDecl &formalsDecl);
  formalsList(formalsDecl &formalsDecl, formalsList &formalsList);
};

class formalsDecl : Node {
 public:
  formalsDecl(typeAnnotation &typeAnnotation, type &type, strint id);
};

class statements : Node {
 public:
  statements(statement &statement);
  statements(statements &statements, statement &statement);
};

class statement : Node {
 public:
  statement(OpenStatement &OpenStatement);
  statement(ClosedStatement &ClosedStatement);
};

class OpenStatement : Node {
 public:
  OpenStatement(string keyWord, exp &exp, statement &statement);
  OpenStatement(string keyWord, exp &exp, ClosedStatement &ClosedStatement, OpenStatement &OpenStatement);
  OpenStatement(string keyWord, exp &exp, OpenStatement &OpenStatement);
};

class ClosedStatement : Node {
 public:
  ClosedStatement(SimpleStatement &SimpleStatement);
  ClosedStatement(string keyWord, exp &exp, ClosedStatement &ClosedStatement, OpenStatement &OpenStatement);
  ClosedStatement(string keyWord, exp &exp, ClosedStatement &ClosedStatement);
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

class call : Node {
 public:
  call(string id, expList &expList);
  call(string id);
};

class expList : Node {
 public:
  expList(exp &exp);
  expList(exp &exp, expList &expList);
};

class type : Node {
 public:
  type(string type);
};

class typeAnnotation : Node {
 public:
  typeAnnotation();
  typeAnnotation(string annoType);
};

class exp : Node {
 public:
  exp(exp &exp);
  exp(exp &exp, string op, exp &exp);
  exp(string id);
  exp(call &call);
  exp(int val, bool isB = false);
  exp(bool val);
  exp(string op, exp &exp);
  exp(typeAnnotation &typeAnnotation, type &type, exp &exp);
};

#endif //COMPIWET3__SEMANTICS_H_
