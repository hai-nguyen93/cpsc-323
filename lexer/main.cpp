// Hai Nguyen
// CPSC 323
// Assignment 1

// This program read a source code file and break it into tokens and lexemes 
// and print the result to the console screen and "analysis.txt" file.

// Progress: this programs works according to the requirements of the assignment

#include <iostream>
#include <fstream>
#include <string>
#include "LexicalAnalyzer.h"
using namespace std;

string getFileName();

int main() {
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
	}
	
	cout << endl;
	system("pause");
	return 0;
}

string getFileName() {
	string fName;
	cout << "Enter file name: ";
	cin >> fName;
	return fName;
}
