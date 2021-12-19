#include "Semantics.h"
#include "hw3_output.hpp"
/*
vector<symbolTable> globSymTable;
vector<int> offsetStack;
*/

symbolRow::symbolRow(string name, int pos, vector<string> types) : name(name), pos(pos), types(types)
{}

program::program() : Node() {
  globSymTable.pop_back();
  offsetStack.pop_back();
}

type::type(string typeName){
    if (typeName == "INT") {
        typeName = "INT";
    } else if (typeName == "BYTE") {
        typeName = "BYTE";
    } else if (typeName == "BOOL") {
        typeName = "BOOL";
    } else{
        //TODO: print desired error
    }
}

typeAnnotation::typeAnnotation(string annoType) : annoType(annoType){};

SimpleStatement::SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id) {
    if(typeAnnotation.annoType == ""){
        //Create an insert new symbolRow:
        vector < string > type_vec = {type.typeName};
        symbolRow new_var(id, *offsetStack.end(), type_vec);
        globSymTable.end()->symbolTable.push_back(new_var);

        //Increasing the current offset by 1:
        int new_offset = *offsetStack.end();
        offsetStack.pop_back();
        offsetStack.push_back(new_offset);
    } else{
        //In case of const this rule is illegal
        //TODO: print desired error in case of const
    }
}

SimpleStatement::SimpleStatement(typeAnnotation &typeAnnotation, type &type, string id, exp &exp) {
    //Create an insert new symbolRow:
    vector < string > type_vec = {type.typeName};
    symbolRow new_var(id, *offsetStack.end(), type_vec);
    globSymTable.end()->symbolTable.push_back(new_var);

    //Increasing the current offset by 1:
    int new_offset = *offsetStack.end();
    offsetStack.pop_back();
    offsetStack.push_back(new_offset);
}
