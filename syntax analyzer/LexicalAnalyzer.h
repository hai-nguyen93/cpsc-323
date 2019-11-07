// Hai Nguyen
// CPSC 323
// Lexical Analyzer class

#pragma once
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const int KEYWORDS_SIZE = 20;
const string KEYWORDS[KEYWORDS_SIZE] = {"int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while", "whileend",
										"do", "doend", "for", "forend", "input", "output", "and", "or", "function" };

const int OPERATORS_SIZE = 8;
const char OPERATORS[OPERATORS_SIZE] = {'*', '+', '-', '=', '/', '>', '<', '%'};

const int SEPARATORS_SIZE = 14;
const char SEPARATORS[SEPARATORS_SIZE] = { '\'', '(', ')', '{', '}', '[', ']', ',', '.', ':', ';', '!', ' ', '\t' };

/* FSM for analyzing identifiers, int, real, and '.'
	inputs = letter(l), digit(d), '$', '.'
	Q:
		q0 = 0: starting state
		1: identifier
		2: int
		3: real
		4: seperator '.'
		5: undefined
		special states: (handling seperator '.')
			6: add the identifier and '.' to the lists, reset the FSM and analyze the rest of the string		
			7: add the '.' to the lists, reset FSM and analyze the rest of the string
			8: add the undefined string and '.' to the lists, reset the FSM and analyze the rest of the string
			9: add the real number and '.' to the lists, reset the FSM and analyze the rest of the string	
	F = {1, 2, 3, 4, 5} 
*/
const int FSM[6][4] = {
 //N:	 l  d  $  . 
//       0  1  2  3
/*0*/	{1, 2, 5, 4},
/*1*/	{1, 1, 1, 6},
/*2*/	{5, 2, 5, 3},
/*3*/	{5, 3, 5, 9},
/*4*/	{7, 3, 7, 7},
/*5*/	{5, 5, 5, 8}
};



class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	void lexer(ifstream& sourceCode); // analyze the source file
	void lexer(string codeLine); // analyze 1 line
	void printResult(string fName); // output the analysis to file

	vector<string> getTokens(); // return the tokens vector
	vector<string> getLexemes(); // return the lexemes vector
	vector<int> getLineNumber(); // return the line_number vector

private:
	vector<string> tokens;
	vector<string> lexemes;
	vector<int> line_number;

	bool isDigit(char ch);
	bool isLetter(char ch);
	bool isKeyword(string str);
	bool isOperator(char ch);
	bool isSeparator(char ch);
	void analyze(string word, int line); // analyze a string using the FSM above
	int charToCol(char ch);
};

