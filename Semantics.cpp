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
	vector < string > print_vec = ["VOID", "STRING"];
	symbolRow print_our("print", 0, print_vec, false, {});
	vector < string > printi_vec = ["VOID", "INT"];
	symbolRow printi_our("printi", 0, printi_vec, false, {});
	new_scope.symbolTable.push_back(print_our, printi_our)
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
	endScope();
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

bool isIdentifierExists(string id) {
	//todo:: implement
}

string findIdentifierType(string id){
	//todo:: implement
}
bool isIdentifierConst(string id){
	//todo:: implement
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
	for (auto formal: formals.formalsList) {
		funcTypes.push_back(formal.type);
		funcConstTypes.push_back(formal.isConst);
	}
	symbolRow symbol_row(id, 0, funcTypes, false, funcConstTypes, true);
	globSymTable.end()->symbolTable.push_back(symbol_row);
	end_scope();
}

retType::retType(type &type) : Node("retType") {
	this->typeName = type;
}

retType::retType(string typeName) : Node("retType") {
	if (typeName != "VOID") {
		output::errorSyn(lineno);
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
				output::errorDef(lineno, id);
				exit(0);
			}
		}
	}
	i = -1;
	for (auto it: formals.formalsList) {
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
		output::errorSyn(lineno);
		exit(0);
	}
	if (exp.type != "BOOL") {
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
	if (exp.type != "BOOL") {
		output::errorMismatch(lineno);
		exit(0);
	}
	end_scope();
}

OpenStatement::OpenStatement(string keyWord, exp &exp, OpenStatement &OpenStatement) : Node("OpenStatement") {
	if (firstKeyWord != "WHILE") {
		output::errorSyn(lineno);
		exit(0);
	}
	if (exp.type != "BOOL") {
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
	if (exp.type != "BOOL") {
		output::errorMismatch(lineno);
		exit(0);
	}
	end_scope();
}

ClosedStatement::ClosedStatement(string keyWord, exp &exp, ClosedStatement &ClosedStatement) : Node("ClosedStatement") {
	if (firstKeyWord != "WHILE") {
		output::errorSyn(lineno);
		exit(0);
	}
	if (exp.type != "BOOL") {
		output::errorMismatch(lineno);
		exit(0);
	}
	end_scope();
}

SimpleStatement::SimpleStatement(string cmd) : Node("SimpleStatement") {
	switch (cmd) {
		case "RETURN":

			break;
		case "BREAK":
			break;
		case "CONTINUE":
			break;
	}
} //return VOID, break, continue

SimpleStatement::SimpleStatement(statements &statements) : Node("SimpleStatement") {
	end_scope();
}	//LBRACE m_newScope statements RBRACE

SimpleStatement::SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id) : Node("SimpleStatement") {
	if (isIdentifierExists(id)) {
		output::errorDef(lineno, id);
		exit(0)
	}
	if(typeAnnotation.isConst){
		output::errorConstDef(lineno);
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
		output::errorDef(lineno, id);
		exit(0)
	}
	if(type.typeName != exp.type){
		output::errorMismatch(lineno);
		exit(0);
	}
	int pos = offsetStack.end() + 1;
	offsetStack.end() = pos;
	symbolRow newIdentifier(id, pos, {type}, typeAnnotation.isConst, {}, false);
	globSymTable.end()->symbolTable.push_back(newIdentifier);
}	//typeAnnotation type ID ASSIGN exp SC

SimpleStatement::SimpleStatement(string id, string assign="ASSIGN", exp &exp) : Node("SimpleStatement") {
	if (!isIdentifierExists(id)) {
		output::errorUndef(lineno, id);
		exit(0)
	}
	string idType = findIdentifierType(id);
	if(idType != exp.type){
		output::errorMismatch(lineno);
		exit(0);
	}
	if (isIdentifierConst(id)){
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

SimpleStatement::SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id) {
	//TODO - firs need to check if the id already exist, and than insert it
	//Check existence to prevent shadowing
	bool alreadyExist = false;
	vector<symbolTable>::iterator it = globSymTable.end();
	for (it; it != globSymTable.begin(); it--) {
		alreadyExist = it->contains(id, {type.typeName});
		if (alreadyExist) {
			break;
		}
	}
	if (!alreadyExist) { //Case the variable not already exist
		if (typeAnnotation.annoType.empty()) {
			//Create an insert new symbolRow:
			vector<string> type_vec = {type.typeName};
			symbolRow new_var(id, *offsetStack.end(), type_vec);
			globSymTable.end()->symbolTable.push_back(new_var);

			//Increasing the current offset by 1:
			int new_offset = *offsetStack.end();
			offsetStack.pop_back();
			offsetStack.push_back(new_offset);
		} else {
			//In case of const this rule is illegal
			//TODO: print desired error in case of const
		}
	} else {
		hw3_output::errorDef(yylino, id);
	}
}

SimpleStatement::SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id, exp &exp) {
	//TODO - firs need to check if the id already exist, and than insert it
	//Check existence to prevent shadowing
	bool alreadyExist = false;
	vector<symbolTable>::iterator it = globSymTable.end();
	for (it; it != globSymTable.begin(); it--) {
		alreadyExist = it->contains(id, {type.typeName});
		if (alreadyExist) {
			break;
		}
	}
	if (!alreadyExist) { //Case the variable not already exist
		//Create an insert new symbolRow:
		vector<string> type_vec = {type.typeName};
		symbolRow new_var(id, *offsetStack.end(), type_vec);
		globSymTable.end()->symbolTable.push_back(new_var);

		//Increasing the current offset by 1:
		int new_offset = *offsetStack.end();
		offsetStack.pop_back();
		offsetStack.push_back(new_offset);
	} else {
		hw3_output::errorDef(yylino, id);
	}
}

SimpleStatement::SimpleStatement(string
								 id,
								 exp &exp
) {

}


