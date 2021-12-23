#ifndef COMPIWET3_TOKENSCLASSES_H
#define COMPIWET3_TOKENSCLASSES_H

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include "Semantics.h"
using namespace std;

//======================================token's Classes=====================================//

class Type : public Node {
	string type;
  public:
	Type(string type) : type(type) {}
};

class Op : public Node {
	string op;
  public:
	Op(string op) : op(op) {}
};

class Bool : public Node {
	bool val;
  public:
	Bool(string boolType) {
		val = boolType == "true";
	}
};

class Cmd : public Node {
	string keyWord;
  public:
	Cmd(string keyWord) : keyWord(keyWord) {}
};

class Parenthesis : public Node {
	string type;
  public:
	Parenthesis(string parenType) : type(parenType) {}
};

class Annotation : public Node {
  public:
	string annoType;
	Annotation(string annoType) : annoType(annoType) {}
};

class Num : public Node {
  public:
	string str;
	Num(std::string strNum) : str(strNum) {}
};

class Id : public Node {
  public:
	std::string IdStr;
	Id(std::string strId) : IdStr(strId) {}
};

class Str : public Node {
  public:
	std::string str;
	Str(std::string strVal) : str(strVal) {}
};

#endif //COMPIWET3_TOKENSCLASSES_H
