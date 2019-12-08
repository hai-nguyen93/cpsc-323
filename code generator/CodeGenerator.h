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
	vector<int> jumpStack;
	string typeToAdd;
	string relOpToAdd;

	// functions
	CodeGenerator();
	~CodeGenerator();

	// symbol table functions
	bool addSymbol(string id, string valueType); 
	int findSymbol(string id); // find location of symbol, -1 if not found
	void printSymbolTable(ofstream& writer) const;
	string getSymbolAddress(int index) const; // get memory address of symbol, using symbol's index in symbolTable

	// assembly code functions
	void generateCode(string com, string operand);
	void printCode() const;
	void backPatch(int jumpAddress);
};

