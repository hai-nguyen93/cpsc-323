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

string CodeGenerator::getSymbolAddress(int index) const {
	return symbolTable[index].address;
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

void CodeGenerator::printSymbolTable(ofstream& writer) const {
	cout << setw(12) << left << "IDENTIFIER" << setw(8) << right << "ADDRESS" << setw(8) << right << "TYPE" << endl;
	writer << setw(12) << left << "IDENTIFIER" << setw(8) << right << "ADDRESS" << setw(8) << right << "TYPE" << endl;
	for (int i = 0; i < symbolTable.size(); ++i) {
		cout << setw(12) << left << symbolTable[i].identifier << setw(8) << right
			<< symbolTable[i].address << setw(8) << right << symbolTable[i].type << endl;
		writer << setw(12) << left << symbolTable[i].identifier << setw(8) << right
			<< symbolTable[i].address << setw(8) << right << symbolTable[i].type << endl;
	}
	cout << endl;
	writer << endl;
}

void CodeGenerator::generateCode(string com, string operand) {
	Code c;
	c.command = com;
	c.operand = operand;
	c.line = assemblyCodes.size() + 1;
	assemblyCodes.push_back(c);
}

void CodeGenerator::printCode() const {
	ofstream writer("assembly_code.txt");

	cout << endl << assemblyCodes.size() << endl;
	for (int i = 0; i < assemblyCodes.size(); ++i) {
		cout << setw(5) << left << assemblyCodes[i].line
			<< setw(8) << left << assemblyCodes[i].command
			<< assemblyCodes[i].operand << endl;
		writer << setw(5) << left << assemblyCodes[i].line
			<< setw(8) << left << assemblyCodes[i].command
			<< assemblyCodes[i].operand << endl;
	}
	cout << endl << endl;
	writer << endl << endl;

	printSymbolTable(writer);
	writer.close();
}

