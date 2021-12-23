#include "Semantics.h"
#include "hw3_output.hpp"

symbolRow::symbolRow(string name,
					 int pos,
					 vector<string> types,
					 bool is_const,
					 vector<bool> constFormals,
					 bool isFunc = false)
	: name(name),
	  pos(pos),
	  types(types),
	  is_const(is_const),
	  constFormals(constFormals),
	  isFunc(isFunc) {}

bool symbolRow::operator==(symbolRow &other) {
	return (this->name == other.name);// && (this->types == other.types);
}

bool symbolTable::contains(string id, vector<string> type) {
	symbolRow dummy(id, -1, type, dummyConst, {});
	bool res = false;
	vector<symbolRow>::iterator it = this->symbolTable.begin();
	for (it; it != this->symbolTable.end(); it++) {
		res = (*it == dummy);
		if (res) {
			return res;
		}
	}
	return res;
}

void m_glob() {
	symbolTable new_scope;
	vector<string> print_vec = {"VOID", "STRING"};
	symbolRow print_our("print", 0, print_vec, false, {});
	vector<string> printi_vec = {"VOID", "INT"};
	symbolRow printi_our("printi", 0, printi_vec, false, {});
	new_scope.symbolTable.push_back(print_our);
	new_scope.symbolTable.push_back(printi_our);
	globSymTable.push_back(new_scope);
	offsetStack.push_back(0);
	return;
}

void m_newScope() {
	//Create and insert new scope table
	symbolTable new_scope;
	globSymTable.push_back(new_scope);
	//Copy the last offset and insert as new offset on top of the stack
	offsetStack.push_back(*offsetStack.end());
}

void m_newScopeWhile() {
	//Create and insert new scope table
	symbolTable new_scope;
	new_scope.isWhileScope = true;
	globSymTable.push_back(new_scope);
	//Copy the last offset and insert as new offset on top of the stack
	offsetStack.push_back(*offsetStack.end());
}

void m_endScope() {
	end_scope();
}

void end_scope() {
	output::endScope();
	symbolTable table = globSymTable.pop_back();
	int offset = offsetStack.pop_back();
	for (auto it: table.symbolTable) {
		if (it.types.size() == 1) {
			//ID
			output::printID(it.name, it.pos, it.types[0]);
		} else {
			//FUNCTION
			string retVal = it.types[0];
			it.types.erase(it.types.begin());
			string funcType = output::makeFunctionType(retVal, it.types);
			output::printID(it.name, 0, funcType);
		}
	}
}

symbolRow findSymbolRow(string id) {
	symbolRow res = symbolRow("", -1, {""}, false, {false});
	for (auto itGlob = globSymTable.rbegin(); itGlob != globSymTable.rend(); itGlob++) {
		for (auto itScope = itGlob.symbolTable.rbegin(); itScope != itGlob.symbolTable.rend(); itScope++) {
			if (itScope.name == id) {
				res = itScope;
				return res;
			}
		}
	}
	return res;
}

bool isIdentifierExists(string id) {
	bool res = false;
	symbolRow check = findSymbolRow(id);
	if (check.name == id) {
		res = true;
	}
	return res;
}

vector<string> findIdentifierType(string id) {
	vector<string> types = {};
	symbolRow identifier = findSymbolRow(id);
	if (!identifier.name.empty()) {
		types = identifier.types;
	}
	return types;
}

bool isIdentifierConst(string id) {
	bool res = false;
	symbolRow check = findSymbolRow(id);
	if (check.name == id) {
		res = check.is_const;
	}
	return res;
}

symbolRow findFunctionRow(string id) {
	//TODO::implement
}
bool isInWhile() {
	//TODO::implement
}
//////////////////////////////////////////////////

program::program() : Node("program") {
	end_scope;
	if (!mainExits) {
		output::errorMainMissing();
	}
}

funcs::funcs() : Node("funcs") {
//todo:: nothing?
}

funcsDecl::funcsDecl(retType &retType, string id, formals &formals, statements &statements) : Node("funcsDecl") {
	if (id == "main") {
		if (mainExits) {
			output::errorDef(yylineno, id);
			exit(0);
		}
		mainExits = true;
	}
	if (isIdentifierExists(id)) {
		output::errorDef(yylineno, id);
		exit(0);
	}
	if (formals.hasString && id != "print") {
		output::errorMismatch(yylineno);
		exit(0);
	}
	vector<string> funcTypes;
	vector<bool> funcConstTypes;
	funcTypes.push_back(retType.typeName);
	for (auto formal: formals.formalsList) {
		funcTypes.push_back(formal.type);
		funcConstTypes.push_back(formal.isConst);
	}
	symbolRow symbol_row(id, 0, funcTypes, false, funcConstTypes, true);
	globSymTable.end()->symbolTable.push_back(symbol_row);
	end_scope();
}

retType::retType(type &type) : Node("retType") {
	this->typeName = type.typeName;
}

retType::retType(string typeName) : Node("retType") {
	if (typeName != "VOID") {
		output::errorSyn(yylineno);
		exit(0);
	}
	this.typeName = typeName;
}

formals::formals() : Node("formals") {
	this->formalsList = {};
}

formals::formals(formalsList &formals) : Node("formals") {
	for (auto formal: formals.formalsList) {
		for (auto nextFormal: formals.formalsList) {
			if (formal == nextFormal) {
				continue;
			}
			if (formal.id == nextFormal.id) {
				output::errorDef(yylineno, id);
				exit(0);
			}
		}
	}
	int i = -1;
	for (auto it: formals.formalsList) {
		if (it.type == "STRING") {
			this->hasString = true;
		}
		symbolRow formal(it.id, i, {it.type}, true, {}, false);
		globSymTable.end()->symbolTable.push_back(formal);
		i--;
	}
	this->formalsList = formals;
}

formalsList::formalsList(formalsDecl &formalsDecl) : Node("formalsList") {
	this->formalsList.push_back(formalsList);
}

formalsList::formalsList(formalsDecl &formalsDecl, formalsList &formalsList) : Node("formalsList") {
	this->formalsList.push_back(formalsDecl);
	this->formalsList.insert(this->formalsList.end(), formalsList.formalsList.begin(), formalsList.formalsList.end());
}

formalsDecl::formalsDecl(typeAnnotation &typeAnnotation, type &type, string id) : Node("formalsDecl") {
	this->id = id;
	this->type = type;
	this->isConst = typeAnnotation;
}

statements::statements(statement &statement) : Node("statements") {
	this->vecStatements.push_back(statement);
}

statements::statements(statements &statements, statement &statement) : Node("statements") {
	this->vecStatements = statements.vecStatements;
	this->vecStatements.push_back(statement);
}

statement::statement(OpenStatement &OpenStatement) : Node("statement") {
	//todo:: nothing?
}

statement::statement(ClosedStatement &ClosedStatement) : Node("statement") {
	//todo:: nothing?
}

OpenStatement::OpenStatement(string keyWord, exp &exp, statement &statement) : Node("OpenStatement") {
	if (keyWord != "IF") {
		output::errorSyn(yylineno);
		exit(0);
	}
	if (exp.type != "BOOL") {
		output::errorMismatch(yylineno);
		exit(0);
	}
	end_scope();
}

OpenStatement::OpenStatement(string firstKeyWord,
							 exp &exp,
							 ClosedStatement &ClosedStatement,
							 string secondKeyWord,
							 OpenStatement &OpenStatement) : Node("OpenStatement") {
	if (firstKeyWord != "IF" || secondKeyWord != "ELSE") {
		output::errorSyn(yylineno);
		exit(0);
	}
	if (exp.type != "BOOL") {
		output::errorMismatch(yylineno);
		exit(0);
	}
	end_scope();
}

OpenStatement::OpenStatement(string keyWord, exp &exp, OpenStatement &OpenStatement) : Node("OpenStatement") {
	if (firstKeyWord != "WHILE") {
		output::errorSyn(yylineno);
		exit(0);
	}
	if (exp.type != "BOOL") {
		output::errorMismatch(yylineno);
		exit(0);
	}
	end_scope();
}

ClosedStatement::ClosedStatement(SimpleStatement &SimpleStatement) : Node("ClosedStatement") {
	//todo:: nothing?
}

ClosedStatement::ClosedStatement(string firstKeyWord, exp &exp, ClosedStatement &ClosedStatement,
								 string secondKeyWord, OpenStatement &OpenStatement) : Node("ClosedStatement") {
	if (firstKeyWord != "IF" || secondKeyWord != "ELSE") {
		output::errorSyn(yylineno);
		exit(0);
	}
	if (exp.type != "BOOL") {
		output::errorMismatch(yylineno);
		exit(0);
	}
	end_scope();
}

ClosedStatement::ClosedStatement(string keyWord, exp &exp, ClosedStatement &ClosedStatement) : Node("ClosedStatement") {
	if (firstKeyWord != "WHILE") {
		output::errorSyn(yylineno);
		exit(0);
	}
	if (exp.type != "BOOL") {
		output::errorMismatch(yylineno);
		exit(0);
	}
	end_scope();
}

SimpleStatement::SimpleStatement(string cmd) : Node("SimpleStatement") {
	switch (cmd) {
		case "RETURN": symbolRow func = findFunctionRow();
			if (func.types[0] != "VOID") {
				output::errorMismatch(yylineno);
				exit(0);
			}
			break;
		case "BREAK":
			if (!isInWhile()) {
				output::errorUnexpectedBreak(yylineno);
				exit(0);
			}
			break;
		case "CONTINUE":
			if (!isInWhile()) {
				output::errorUnexpectedContinue(yylineno);
				exit(0);
			}
			break;
	}
} //return VOID, break, continue

SimpleStatement::SimpleStatement(statements &statements) : Node("SimpleStatement") {
	end_scope();
}    //LBRACE m_newScope statements RBRACE

SimpleStatement::SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id) : Node("SimpleStatement") {
	if (isIdentifierExists(id)) {
		output::errorDef(yylineno, id);
		exit(0);
	}
	if (typeAnnotation.isConst) {
		output::errorConstDef(yylineno);
		exit(0);
	}
	int pos = offsetStack.end() + 1;
	offsetStack.end() = pos;
	symbolRow newIdentifier(id, pos, {type}, typeAnnotation.isConst, {}, false);
	globSymTable.end()->symbolTable.push_back(newIdentifier);
} //typeAnnotation type ID SC

SimpleStatement::SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id, exp &exp) : Node(
	"SimpleStatement") {
	if (isIdentifierExists(id)) {
		output::errorDef(yylineno, id);
		exit(0);
	}
	if (type.typeName != exp.type) {
		output::errorMismatch(yylineno);
		exit(0);
	}
	int pos = offsetStack.end() + 1;
	offsetStack.end() = pos;
	symbolRow newIdentifier(id, pos, {type}, typeAnnotation.isConst, {}, false);
	globSymTable.end()->symbolTable.push_back(newIdentifier);
}    //typeAnnotation type ID ASSIGN exp SC

SimpleStatement::SimpleStatement(string id, string assign, exp &exp) : Node("SimpleStatement") {
	if (assign != "ASSIGN") {
		output::errorSyn(yylineno);
		exit(0);
	}
	if (!isIdentifierExists(id)) {
		output::errorUndef(yylineno, id);
		exit(0);
	}
	vector<string> idType = findIdentifierType(id);
	if (idType.size() != 1 || idType[0] != exp.type) {
		output::errorMismatch(yylineno);
		exit(0);
	}
	if (isIdentifierConst(id)) {
		output::errorConstMismatch(yylineno);
		exit(0);
	}
}//ID ASSIGN exp SC

SimpleStatement::SimpleStatement(call &call) : Node("SimpleStatement") {
	//todo:: nothing?
}

SimpleStatement::SimpleStatement(exp &exp) : Node("SimpleStatement") {
	symbolRow func = findFunctionRow();
	if (func.types[0] != exp.type) {
		output::errorMismatch(yylineno);
		exit(0);
	}
}//RETURN exp SC

call::call(string id, expList &expList) : Node("call") {
	symbolRow funcId = findSymbolRow(id);
	if (id != funcId.name || !funcId.isFunc) {
		output::errorUndefFunc(yylineno, id);
		exit(0);
	}
	if (funcId.types.size() != expList.expVector.size() + 1) {
		output::errorPrototypeMismatch(yylineno, id, funcId.types);
		exit(0);
	}
	for (int i = 1; i < funcId.types.size(); i++) {
		if (funcId.types[i] != expList.expVector[i - 1].type) {
			if (id == "printi") {
				if (expList.expVector[i - 1].type != "BYTE") {
					output::errorPrototypeMismatch(yylineno, id, funcId.types);
					exit(0);
				}
			} else {
				output::errorPrototypeMismatch(yylineno, id, funcId.types);
				exit(0);
			}
		}
	}
	this->rettype = funcId.types[0];
}

call::call(string id) : Node("call") {
	symbolRow funcId = findSymbolRow(id);
	if (id != funcId.name || !funcId.isFunc) {
		output::errorUndefFunc(yylineno, id);
		exit(0);
	}
	if (funcId.types.size() != 1) {
		output::errorPrototypeMismatch(yylineno, id, funcId.types);
		exit(0);
	}
	this->rettype = funcId.types[0];
}

expList::expList(exp &exp) : Node("expList") {
	this->expVectorexpVector.push_back(exp);
}

expList::expList(exp &exp, expList &expList) : Node("expList") {
	this->expVector.push_back(exp);
	this->expVector.insert(this->expVector.end(), expList.begin(), expList.end());
}

type::type(string typeName) : Node("type") {
	if (typeName == "INT") {
		typeName = "INT";
	} else if (typeName == "BYTE") {
		typeName = "BYTE";
	} else if (typeName == "BOOL") {
		typeName = "BOOL";
	} else {
		output::errorSyn(yylineno);
		exit(0);
	}
}

typeAnnotation::typeAnnotation(string annoType) : Node("typeAnnotation") {
	if (annoType == "") {
		isConst = false;
	} else if (annoType == "CONST") {
		isConst = true;
	} else {
		output::errorSyn(yylineno);
		exit(0);
	}
}

exp::exp(exp &exp) : Node("exp") {
	this->type = exp.type;
}

exp::exp(exp &firstExp, string op, exp &secExp) : Node("exp") {
	if (op == "MULT" || op == "DIV" || op == "PLUS" || op == "MINUS") {
		if ((firstExp.type != "INT" && firstExp.type != "BYTE") || (secExp.type != "INT" && secExp.type != "BYTE")) {
			output::errorMismatch(yylineno);
			exit(0);
		}
		if (firstExp.type == "INT" || secExp.type == "INT") {
			this->type = "INT";
		} else {
			this->type = "BYTE";
		}
	} else if (op == "AND" || op == "OR" || op == "RELOPLEFT" || op == "RELOPNONASSOC") {
		if ((firstExp.type != "BOOL") || (secExp.type != "BOOL")) {
			output::errorMismatch(yylineno);
			exit(0);
		}
		this->type = "BOOL";
	} else {
		output::errorSyn(yylineno);
		exit(0);
	}
}//MULT,DIV,PLUS,MINUS,AND,OR

exp::exp(string id, string type) : Node("exp") {
	if (type == "ID") {
		symbolRow res = findSymbolRow(id);
		if (res.name != id) {
			output::errorUndef(yylineno, id);
			exit(0);
		}
		if (res.isFunc) {
			output::errorUndef(yylineno, id);
			exit(0);
		}
		this->type = res.types[0];
	} else if (type == "STRING") {
		this->type = "STRING";
	}

} //ID,STRING

exp::exp(call &call) : Node("exp") {
	this->type = call.rettype;
}//call

exp::exp(int val, bool isB = false) : Node("exp") {
	if (isB) {
		if (val > 255) {
			string value = std::to_string(val);
			output::errorByteTooLarge(lineno, value);
			exit(0);
		}
		this->type = "BYTE";
	} else {
		this->type = "INT";
	}
}//INT,BYTE

exp::exp(bool val) : Node("exp") {
	this->type = "BOOL";
}

exp::exp(string op, exp &exp) : Node("exp") {
	if (exp.type != "BOOL") {
		output::errorMismatch(yylineno);
		exit(0);
	}
	this->type = "BOOL";
}

exp::exp(typeAnnotation &typeAnnotation, type &type, exp &exp) : Node("exp") {
	if ((type.typeName != "INT" && type.typeName != "BYTE") || (exp.type != "INT" && exp.type != "BYTE")) {
		output::errorMismatch(yylineno);
		exit(0);
	}
	this->type = type.typeName;
}



