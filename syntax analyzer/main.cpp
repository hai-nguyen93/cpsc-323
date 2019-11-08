// Hai Nguyen
// CPSC 323
// Assignment 2
// Syntax analysis
// This program reads a source code file or 1 line of code and analyze the syntax (predictive RDP).
// The result is printed to the screen and "parse_result.txt" file

// Progress: this program can parse:
// - Arithmetic assignment statement: a= b+c;
// - Arithmetic expression statement: 1 * a;
// - Declare statement: int a, b;
// - If statement:
//		if a > b then {
//			a = b;
//		} else {
//			a = 1;
//			b = 2;
//		} endif


#include <iostream>
#include <fstream>
#include <string>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
using namespace std;

string getFileName() {
	string fName;
	cout << "Enter file name: ";
	cin >> fName;
	return fName;
}

char getChar(string prompt) {
	char c;
	cout << prompt;
	cin >> c;
	cin.ignore(999, '\n');
	return c;
}

void testFile() {
	string fName = getFileName();
	ifstream file(fName);
	if (!file.is_open()) {
		cout << "File not found." << endl;
	}
	else {
		LexicalAnalyzer la;
		la.lexer(file);
		file.close();
		la.printResult("analysis.txt");
		SyntaxAnalyzer sa;
		sa.analyze(la.getTokens(), la.getLexemes(), la.getLineNumber());
	}
}

void testLine() {
	string test;
	cout << "Enter 1 line of code: " << endl;
	getline(cin, test);
	if (test.size() <= 0) return;
	LexicalAnalyzer la;
	la.lexer(test);
	la.printResult("analysis.txt");
	SyntaxAnalyzer sa;
	sa.analyze(la.getTokens(), la.getLexemes(), la.getLineNumber());
}

int main() {
	char command;
	do {
		cout << endl << "1 : enter 1 line to test" << endl
					 << "2 : enter a source code file to test" << endl
					 << "q : quit" << endl;
		command = getChar("Enter a command: ");

		switch (command) {
		case '1':
			testLine();
			break;
		case '2':
			testFile();
			command = 'q';
			break;
		case 'q': case 'Q':
			break;
		default:
			cout << "Invalid command." << endl;
			break;
		}
	} while (command != 'q' && command != 'Q');
	
	cout << endl;
	system("pause");
	return 0;
}
