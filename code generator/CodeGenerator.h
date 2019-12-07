#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

struct Symbol {
	string identifier;
	string address;
	string type;
};

struct Code {
	int line;
	string command;
	string operand;
};

class CodeGenerator
{
public:
	// variables
	vector<Symbol> symbolTable;
	vector<Code> assemblyCodes;
	string typeToAdd;

	// functions
	CodeGenerator();
	~CodeGenerator();

	// symbol table functions
	bool addSymbol(string id, string valueType);
	int findSymbol(string id);
	void printSymbolTable() const;

	// assembly code functions
	void generateCode(string com, string op);

};

