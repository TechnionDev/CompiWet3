#include "Semantics.h"

vector<symbolTable> globSymTable;
vector<int> offsetStack;

symbolRow::symbolRow(string name, int pos, vector<string> types) : name(name), pos(pos), types(types)
}

program::program() : Node() {
  globSymTable.pop_back();
  offsetStack.pop_back();
}


