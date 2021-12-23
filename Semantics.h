#ifndef COMPIWET3__SEMANTICS_H_
#define COMPIWET3__SEMANTICS_H_

#include "hw3_output.hpp"
#include <string.h>
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

extern int yylineno;
bool mainExits = false;
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
	symbolRow();
	symbolRow(const symbolRow& row);
	bool operator==(symbolRow &other);
};

class symbolTable {
  public:
	symbolTable();
	vector<symbolRow> SymbolTable;
	bool isWhileScope = false;
	bool contains(string id, vector<string> type);
};

void m_glob();

void m_newScope();

void m_newScopeWhile();

void m_endScope();

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
	vector<formalsDecl> formalsVector;
	bool hasString = false;
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
	string formalType;
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
	SimpleStatement(string id, string assign, exp &exp);
	SimpleStatement(call &call);
	SimpleStatement(exp &exp);

};

class call : Node {
  public:
	string rettype;
	call(string id, expList &expList);
	call(string id);
};

class expList : Node {
  public:
	vector<exp> expVector;
	expList(const exp &exp1);
	expList(const exp &exp1,const expList &expList);
};

class type : Node {
  public:
	string typeName;
	type(string typeName);
};

class typeAnnotation : Node {
  public:
	bool isConst;
	typeAnnotation(string annoType = "");
};

class exp : Node {
  public:
	string expType;
	exp();
	exp(const exp &exp);
	exp(const exp &firstExp, string op,const exp &secExp);
	exp(string id, string type);
	exp(const call &call);
	exp(int val, bool isB = false);
	exp(bool val);
	exp(string op,const exp &exp);
	exp(const typeAnnotation &typeAnnotation,const type &type,const exp &exp);
};

#endif //COMPIWET3__SEMANTICS_H_
