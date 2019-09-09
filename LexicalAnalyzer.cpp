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



class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	void lexer(ifstream& sourceCode);
	void printResult(string fName);

	vector<string> getTokens();
	vector<string> getLexemes();

private:
	vector<string> tokens;
	vector<string> lexemes;

	bool isKeyword(string str);
	bool isOperator(char ch);
	bool isSeparator(char ch);
	void analyze(string word);
};

