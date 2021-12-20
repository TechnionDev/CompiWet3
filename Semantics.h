#ifndef COMPIWET3__SEMANTICS_H_
#define COMPIWET3__SEMANTICS_H_

#include "hw3_output.hpp"
#include "SymbolTable.h"
using namespace std;
class Node;
class symbolTable;
class program;
class funcs;
class funcsDecl;
class retType;
class formals;
class formalsList;
class formalsDecl;
class statements;
class statement;
class OpenStatement;
class ClosedStatement;
class SimpleStatement;
class call;
class expList;
class type;
class typeAnnotation;
class exp;

vector<symbolTable> globSymTable;
vector<int> offsetStack;
bool mainExits = false;
string currentFunctionScope;
#define YYSTYPE Node*

class Node {
  public:
	string val;
	Node() : val("") {};
	Node(string val) : val(val) {};
};

class symbolRow {
	string name;
	int pos;
	vector<string> types;
	bool is_const;
	vector<bool> constFormals;
	bool isFunc = false;
	symbolRow(string name,
			  int pos,
			  vector<string> types,
			  bool is_const,
			  vector<bool> constFormals,
			  bool isFunc = false);
	bool operator==(symbolRow &other);
};

class symbolTable {
  public:
	vector<symbolRow> symbolTable;
	bool contains(string id, vector<string> type);
};

void m_glob();

void m_newScope();

void end_scope();

//////////////////////////////////////////////////

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
	funcsDecl(retType &retType, string id, formals &formals, statements &statements);
};

class retType : Node {
  public:
	string typeName;
	retType(type &type);
	retType(string typeName);
};

class formals : Node {
  public:
	vector<formalsDecl> formalsList;
	formals();
	formals(formalsList &formalsList);
};

class formalsList : Node {
  public:
	vector<formalsDecl> formalsList;
	formalsList(formalsDecl &formalsDecl);
	formalsList(formalsDecl &formalsDecl, formalsList &formalsList);
};

class formalsDecl : Node {
  public:
	string type;
	bool isConst;
	string id;
	formalsDecl(typeAnnotation &typeAnnotation, type &type, string id);
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

#endif //COMPIWET3__SEMANTICS_H_
