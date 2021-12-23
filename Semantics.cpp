#include "Semantics.h"
#include "hw3_output.hpp"

vector<SymbolTable> globSymTable = {};
vector<int> offsetStack = {};
bool mainExits = false;
int lineno = 0;

symbolRow::symbolRow(string name,
					 int pos,
					 vector<string> types,
					 bool is_const,
					 vector<bool> constFormals,
					 bool isFunc)
	: name(name),
	  pos(pos),
	  types(types),
	  is_const(is_const),
	  constFormals(constFormals),
	  isFunc(isFunc) {}

bool symbolRow::operator==(symbolRow &other) {
	return (this->name == other.name);// && (this->types == other.types);
}

SymbolTable::SymbolTable() : symbolTable({}), isWhileScope(false) {}
SymbolTable::SymbolTable(SymbolTable *other) : symbolTable(other->symbolTable), isWhileScope(other->isWhileScope) {
}

bool SymbolTable::contains(string id, vector<string> type) {
	auto dummyConst = false;
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
	SymbolTable new_scope;
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
	SymbolTable new_scope;
	globSymTable.push_back(new_scope);
	//Copy the last offset and insert as new offset on top of the stack
	offsetStack.push_back(*offsetStack.end());
}

void m_newScopeWhile() {
	//Create and insert new scope table
	SymbolTable new_scope;
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
	SymbolTable table = globSymTable.pop_back();
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
	for (auto itGlob: globSymTable) {
		for (auto itScope: itGlob.symbolTable) {
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
//////////////////////////////////////////////////

program::program() : Node("program") {
	void end_scope();
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
			output::errorDef(lineno, id);
			exit(0);
		}
		mainExits = true;
	}
	if (isIdentifierExists(id)) {
		output::errorDef(lineno, id);
		exit(0);
	}

	vector<string> funcTypes;
	vector<bool> funcConstTypes;
	funcTypes.push_back(retType.typeName);
	for (auto formal: formals.formalsVector) {
		funcTypes.push_back(formal.typeName);
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
		output::errorSyn(lineno);
		exit(0);
	}
	this.typeName = typeName;
}

formals::formals() : Node("formals") {
	this->formalsVector = {};
}

formals::formals(formalsList &formals) : Node("formals") {
	for (auto formal: formals.formalsVector) {
		for (auto nextFormal: formals.formalsVector) {
			if (formal == nextFormal) {
				continue;
			}
			if (formal.id == nextFormal.id) {
				output::errorDef(lineno, formal.id);
				exit(0);
			}
		}
	}
	int i = -1;
	for (auto it: formals.formalsVector) {
		symbolRow formal(it.id, i, {it.typeName}, true, {}, false);
		globSymTable.end()->symbolTable.push_back(formal);
		i--;
	}
	this->formalsVector = formals.formalsVector;
}

formalsList::formalsList(formalsDecl &formalsDecl) : Node("formalsVector") {
	this->formalsVector.push_back(formalsVector);
}

formalsList::formalsList(formalsDecl &formalsDecl, formalsList &formalsList) : Node("formalsVector") {
	this->formalsVector.push_back(formalsDecl);
	this->formalsVector.insert(this->formalsVector.end(),
							   formalsList.formalsVector.begin(),
							   formalsList.formalsVector.end());
}

formalsDecl::formalsDecl(typeAnnotation &typeAnnotation, type &type, string id) : Node("formalsDecl") {
	this->id = id;
	this->typeName = type.typeName;
	if (typeAnnotation.annoType == "CONST") {
		this->isConst = true;
	} else {
		this->isConst = false;
	}
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
		output::errorSyn(lineno);
		exit(0);
	}
	if (exp.typeName != "BOOL") {
		output::errorMismatch(lineno);
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
		output::errorSyn(lineno);
		exit(0);
	}
	if (exp.typeName != "BOOL") {
		output::errorMismatch(lineno);
		exit(0);
	}
	end_scope();
}

OpenStatement::OpenStatement(string firstKeyWord, exp &exp, OpenStatement &OpenStatement) : Node("OpenStatement") {
	if (firstKeyWord != "WHILE") {
		output::errorSyn(lineno);
		exit(0);
	}
	if (exp.typeName != "BOOL") {
		output::errorMismatch(lineno);
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
		output::errorSyn(lineno);
		exit(0);
	}
	if (exp.typeName != "BOOL") {
		output::errorMismatch(lineno);
		exit(0);
	}
	end_scope();
}

ClosedStatement::ClosedStatement(string firstKeyWord, exp &exp, ClosedStatement &ClosedStatement) : Node(
	"ClosedStatement") {
	if (firstKeyWord != "WHILE") {
		output::errorSyn(lineno);
		exit(0);
	}
	if (exp.typeName != "BOOL") {
		output::errorMismatch(lineno);
		exit(0);
	}
	end_scope();
}

SimpleStatement::SimpleStatement(string cmd) : Node("SimpleStatement") {
	switch (cmd) { //TODO - switch doesn't work on string
		case "RETURN":break;
		case "BREAK":break;
		case "CONTINUE":break;
	}
} //return VOID, break, continue

SimpleStatement::SimpleStatement(statements &statements) : Node("SimpleStatement") {
	end_scope();
}    //LBRACE m_newScope statements RBRACE

SimpleStatement::SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id) : Node("SimpleStatement") {
	if (isIdentifierExists(id)) {
		output::errorDef(lineno, id);
		exit(0);
	}
	if (typeAnnotation.isConst) {
		output::errorConstDef(lineno);
		exit(0);
	}
	int pos = *offsetStack.end() + 1;
	*offsetStack.end() = pos;
	symbolRow newIdentifier(id, pos, {type.typeName}, typeAnnotation.isConst, {}, false);
	globSymTable.end()->symbolTable.push_back(newIdentifier);
} //typeAnnotation typeName ID SC

SimpleStatement::SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id, exp &exp) : Node(
	"SimpleStatement") {
	if (isIdentifierExists(id)) {
		output::errorDef(lineno, id);
		exit(0);
	}
	if (type.typeName != exp.typeName) {
		output::errorMismatch(lineno);
		exit(0);
	}
	int pos = *offsetStack.end() + 1;
	*offsetStack.end() = pos;
	symbolRow newIdentifier(id, pos, {type.typeName}, typeAnnotation.isConst, {}, false);
	globSymTable.end()->symbolTable.push_back(newIdentifier);
}    //typeAnnotation typeName ID ASSIGN exp SC

SimpleStatement::SimpleStatement(string id, string assign, exp &exp) : Node("SimpleStatement") {
	if (!isIdentifierExists(id)) {
		output::errorUndef(lineno, id);
		exit(0);
	}
	//TODO - I dont understand what you try to do because type is not string but vector<string>
	string idType = findIdentifierType(id);
	if (idType != exp.typeName) {
		output::errorMismatch(lineno);
		exit(0);
	}
	if (isIdentifierConst(id)) {
		output::errorConstMismatch(lineno);
		exit(0);
	}
}//ID ASSIGN exp SC

SimpleStatement::SimpleStatement(call &call) : Node("SimpleStatement") {
	//todo:: nothing?
}

SimpleStatement::SimpleStatement(exp &exp) : Node("SimpleStatement") {

}//RETURN exp SC

type::type(string
		   typeName) {
	if (typeName == "INT") {
		typeName = "INT";
	} else if (typeName == "BYTE") {
		typeName = "BYTE";
	} else if (typeName == "BOOL") {
		typeName = "BOOL";
	} else {
//TODO: print desired error
	}
}

typeAnnotation::typeAnnotation(string
							   annoType) :
	annoType(annoType) {};


