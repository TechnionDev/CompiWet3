#include "Semantics.h"
#include "hw3_output.hpp"

symbolRow::symbolRow(string name, int pos, vector<string> types, bool is_const)
	: name(name),
	  pos(pos),
	  types(types),
	  is_const(is_const) {}

bool symbolRow::operator==(symbolRow &other) {
	return (this->name == other.name);// && (this->types == other.types);
}

bool symbolTable::contains(string id, vector<string> type) {
	symbolRow dummy(id, -1, type);
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
	symbolRow print_our("print", 0, print_vec, false);
	vector < string > printi_vec = ["VOID", "INT"];
	symbolRow printi_our("printi", 0, printi_vec, false);
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

//////////////////////////////////////////////////

program::program() : Node("program") {
	void end_scope();
	if (!mainExits) {
		errorMainMissing();
	}
}

funcs::funcs() : Node("funcs") {
	//nothing?
}

funcsDecl::funcsDecl(retType &type, string id, formals &formals, statements &statements) : Node("funcsDecl") {

}
type::type(string typeName) {
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

typeAnnotation::typeAnnotation(string annoType) : annoType(annoType) {};

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

SimpleStatement::SimpleStatement(string id, exp &exp) {

}

retType::retType(type &type) : typeName(type.typeName) {}

retType::retType(string typeName) : typeName(typeName) {}

formalsDecl::formalsDecl(typeAnnotation &typeAnnotation, type &type, string id) {
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

