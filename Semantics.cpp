#include "Semantics.h"

vector<symbolTable> globSymTable;
vector<int> offsetStack;

symbolRow::symbolRow(string name, int pos, vector<string> types) : name(name), pos(pos), types(types)
}

program::program() : Node() {
  globSymTable.pop_back();
  offsetStack.pop_back();
}

void m_glob() {
  symbolTable new_scope;
  vector < string > print_vec = ["VOID", "STRING"];
  symbolRow print_our("print", 0, print_vec);
  vector < string > printi_vec = ["VOID", "INT"];
  symbolRow printi_our("print", 0, printi_vec);
  new_scope.symbolTable.push_back(print_our, printi_our)
  globSymTable.push_back(new_scope);
  offsetStack.push_back(0);
  return;
}

