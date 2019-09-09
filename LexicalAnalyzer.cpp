#include "LexicalAnalyzer.h"
#include <iostream>
#include <iomanip>

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
			if (buffer.length() > 0) {
				analyze(buffer);
				buffer.clear();
			}

			tokens.push_back("operator");
			lexemes.push_back(string(1, c));
		}
		else if (isSeparator(c) || c == '\n') {
			if (buffer.length() > 0) {
				analyze(buffer);
				buffer.clear();
			}

			if (c != ' ' && c != '\n') {
				tokens.push_back("separator");
				lexemes.push_back(string(1, c));
			}
		}
		else {
			buffer += c;
		}
		
		//cout << buffer.length() << endl;
		cout << c;
	}

	cout << endl << setw(11) << "TOKENS" << "\tLEXEMES" << endl;
	for (int i = 0; i < tokens.size(); ++i) {
		cout << setw(11) << tokens[i] << "\t" << lexemes[i] << endl;
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
	writer << setw(11) << "TOKENS" << "\tLEXEMES" << endl;
	for (int i = 0; i < tokens.size(); ++i) {
		writer << setw(11) << tokens[i] << "\t" << lexemes[i] << endl;
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

void LexicalAnalyzer::analyze(string word) {
	if (isKeyword(word)) {
		tokens.push_back("keyword");
		lexemes.push_back(word);
		return;
	}
	tokens.push_back("identifier");
	lexemes.push_back(word);
}
