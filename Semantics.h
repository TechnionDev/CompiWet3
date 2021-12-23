#ifndef COMPIWET3__SEMANTICS_H_
#define COMPIWET3__SEMANTICS_H_

#include "hw3_output.hpp"
#include <string.h>
using namespace std;
class Node;
class SymbolTable;
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

extern vector<SymbolTable> globSymTable;
extern vector<int> offsetStack;
extern bool mainExits;
extern int lineno;
#define YYSTYPE Node*

class Node {
  public:
	string val;
	Node() : val("") {};
	Node(string val) : val(val) {};
};

class symbolRow {
  public:
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

class SymbolTable {
  public:
	vector<symbolRow> symbolTable;
	bool isWhileScope;
	SymbolTable();
	SymbolTable(SymbolTable *other);
	bool contains(string id, vector<string> type);
};

void m_glob();

void m_newScope();

void m_newScopeWhile();

void m_endScope();

void end_scope();

symbolRow findSymbolRow(string id);
vector<string> findIdentifierType(string id);
bool isIdentifierExists(string id);
bool isIdentifierConst(string id);

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
	vector<formalsDecl> formalsVector;
	formals();
	formals(formalsList &formalsList);
};

class formalsList : Node {
  public:
	vector<formalsDecl> formalsVector;
	formalsList(formalsDecl &formalsDecl);
	formalsList(formalsDecl &formalsDecl, formalsList &formalsList);
};

class formalsDecl : Node {
  public:
	string typeName;
	bool isConst;
	string id;
	formalsDecl(typeAnnotation &typeAnnotation, type &type, string id);
};

class statements : Node {
  public:
	vector<statement> vecStatements;
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
	OpenStatement(string firstKeyWord,
				  exp &exp,
				  ClosedStatement &ClosedStatement,
				  string secondKeyWord,
				  OpenStatement &OpenStatement);
	OpenStatement(string keyWord, exp &exp, OpenStatement &OpenStatement);
};

class ClosedStatement : Node {
  public:
	ClosedStatement(SimpleStatement &SimpleStatement);
	ClosedStatement(string firstKeyWord,
					exp &exp,
					ClosedStatement &ClosedStatement,
					string secondKeyWord,
					OpenStatement &OpenStatement);
	ClosedStatement(string keyWord, exp &exp, ClosedStatement &ClosedStatement);
};

class SimpleStatement : Node {
  public:
	SimpleStatement(string cmd); //return, break, continue
	SimpleStatement(statements &statements);
	SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id);
	SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id, exp &exp);
	SimpleStatement(string id,
					string assign = "ASSIGN",
					exp &exp); //TODO - You can't define parameter without default after one with default
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
	bool isConst;
	typeAnnotation(string annoType = "");
};

class exp : Node {
  public:
	string typeName;
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
