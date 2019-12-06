#include "CodeGenerator.h"



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

int CodeGenerator::symbolExist(string id) {
	for (int i = 0; i < symbolTable.size(); ++i) {
		if (symbolTable[i].identifier == id)
			return i;
	}
	return -1;
}

void CodeGenerator::addSymbol(string id, string valueType) {
	Symbol s;
	s.identifier = id;
	s.address = to_string(symbolTable.size() + 5000);
	s.type = valueType;
	symbolTable.push_back(s);
}

void CodeGenerator::generateCode(string com, string op) {
	Code c;
	c.command = com;
	c.operand = op;
	c.line = assemblyCodes.size() + 1;
	assemblyCodes.push_back(c);
}

