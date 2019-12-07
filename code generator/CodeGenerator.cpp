#include "CodeGenerator.h"
#include <iomanip>



CodeGenerator::CodeGenerator()
{
	symbolTable.clear();
	assemblyCodes.clear();
}

CodeGenerator::~CodeGenerator()
{
	symbolTable.clear();
	assemblyCodes.clear();
}

int CodeGenerator::findSymbol(string id) {
	for (int i = 0; i < symbolTable.size(); ++i) {
		if (symbolTable[i].identifier == id)
			return i;
	}
	return -1;
}

bool CodeGenerator::addSymbol(string id, string valueType) {
	if (findSymbol(id) > -1) return false;

	Symbol s;
	s.identifier = id;
	s.address = to_string(symbolTable.size() + 5000);
	s.type = valueType;
	symbolTable.push_back(s);
	return true;
}

void CodeGenerator::printSymbolTable() const {
	cout << setw(12) << left << "IDENTIFIER" << setw(8) << right << "ADDRESS" << setw(8) << right << "TYPE" << endl;
	for (int i = 0; i < symbolTable.size(); ++i) {
		cout << setw(12) << left << symbolTable[i].identifier << setw(8) << right
			<< symbolTable[i].address << setw(8) << right << symbolTable[i].type << endl;
	}
	cout << endl;
}

void CodeGenerator::generateCode(string com, string op) {
	Code c;
	c.command = com;
	c.operand = op;
	c.line = assemblyCodes.size() + 1;
	assemblyCodes.push_back(c);
}

