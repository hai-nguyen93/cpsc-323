#pragma once
#include <vector>
#include <string>
#include <fstream>

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

	// functions
	CodeGenerator();
	~CodeGenerator();

	void addSymbol(string id, string valueType);
	int symbolExist(string id);
	void generateCode(string com, string op);

};

