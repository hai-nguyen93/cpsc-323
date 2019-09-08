#include "LexicalAnalyzer.h"
#include <iostream>

LexicalAnalyzer::LexicalAnalyzer()
{
	tokens.clear();
	lexemes.clear();
}

LexicalAnalyzer::~LexicalAnalyzer()
{
	tokens.clear();
	lexemes.clear();
}

void LexicalAnalyzer::lexer(ifstream& sourceCode) {
	char c;
	string buffer;

	while (sourceCode.get(c)) {
		//if (c == '\n') continue;

		if (isOperator(c)) {
			tokens.push_back("operator");
			lexemes.push_back(string(1, c));
		}

		if (isSeparator(c)) {
			if (c != ' ') {
				tokens.push_back("separator");
				lexemes.push_back(string(1, c));
			}
		}

		buffer += c;

		cout << c;
	}

	cout << endl << "TOKENS \t\t LEXEMES" << endl;
	for (int i = 0; i < tokens.size(); ++i) {
		cout << tokens[i] << "\t" << lexemes[i] << endl;
	}
}

vector<string> LexicalAnalyzer::getTokens() {
	return tokens;
}

vector<string> LexicalAnalyzer::getLexemes() {
	return lexemes;
}

void LexicalAnalyzer::printResult(string fName) {
	ofstream writer(fName);
	writer << "TOKENS \t\t LEXEMES" << endl;
	for (int i = 0; i < tokens.size(); ++i) {
		writer << tokens[i] << "\t" << lexemes[i] << endl;
	}
	writer.close();
}

bool LexicalAnalyzer::isKeyword(string str) {
	for (int i = 0; i < KEYWORDS_SIZE; ++i) 
		if (str == KEYWORDS[i]) return true;

	return false;
}

bool LexicalAnalyzer::isOperator(char ch) {
	for (int i = 0; i < OPERATORS_SIZE; ++i)
		if (ch == OPERATORS[i]) return true;

	return false;
}

bool LexicalAnalyzer::isSeparator(char ch) {
	for (int i = 0; i < SEPARATORS_SIZE; ++i)
		if (ch == SEPARATORS[i]) return true;

	return false;
}