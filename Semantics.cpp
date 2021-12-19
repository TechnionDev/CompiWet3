#include "Semantics.h"
#include "hw3_output.hpp"

/*
vector<symbolTable> globSymTable;
vector<int> offsetStack;
*/

symbolRow::symbolRow(string name, int pos, vector<string> types) : name(name), pos(pos), types(types) {}

bool symbolRow::operator==(symbolRow &other) {
    return (this->name == other.name) && (this->types == other.types);
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

Node::Node() : val("") {}

program::program() : Node() {
    globSymTable.pop_back();
    offsetStack.pop_back();
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

