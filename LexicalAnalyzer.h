// Hai Nguyen
// CPSC 323
// Lexical Analyzer class

#pragma once
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const int KEYWORDS_SIZE = 18;
const string KEYWORDS[KEYWORDS_SIZE] = {"int", "float", "bool", "if", "else", "then", "endif", "while", "whileend",
										"do", "doend", "for", "forend", "input", "output", "and", "or", "function" };

const int OPERATORS_SIZE = 8;
const char OPERATORS[OPERATORS_SIZE] = {'*', '+', '-', '=', '/', '>', '<', '%'};

const int SEPARATORS_SIZE = 13;
const char SEPARATORS[SEPARATORS_SIZE] = { '\'', '(', ')', '{', '}', '[', ']', ',', '.', ':', ';', '!', ' ' };

/* FSM for analyzing identifiers, int, real, and '.'
	input = letter(l), digit(d), '$', '.'
	Q:
		q0 = 0: starting state
		1: identifier
		2: int
		3: real
		4: seperator '.'
		5: undefined
		6: special state: add the identifier and '.' to the lists and continue to analyze to rest of the string
	F = {1, 2, 3, 4} 
	*/
const int FSM[6][4] = {
 //N:	 l  d  $  . 
/*0*/	{1, 2, 5, 4},
/*1*/	{1, 1, 1, 6},
/*2*/	{5, 2, 5, 3},
/*3*/	{5, 3, 5, 5},
/*4*/	{5, 3, 5, 5},
/*5*/	{5, 5, 5, 5}
};



class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	void lexer(ifstream& sourceCode); // analyze the source file
	void printResult(string fName); // output the analysis to file

	vector<string> getTokens();
	vector<string> getLexemes();

private:
	vector<string> tokens;
	vector<string> lexemes;

	bool isDigit(char ch);
	bool isLetter(char ch);
	bool isKeyword(string str);
	bool isOperator(char ch);
	bool isSeparator(char ch);
	void analyze(string word); // analyze a string using the FSM above
};

