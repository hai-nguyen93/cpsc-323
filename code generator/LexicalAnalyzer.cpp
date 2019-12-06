// Hai Nguyen
// CPSC 323
// Implementation of Lexical Analyzer class

#include "LexicalAnalyzer.h"
#include <iostream>
#include <iomanip>

LexicalAnalyzer::LexicalAnalyzer()
{
	tokens.clear();
	lexemes.clear();
	line_number.clear();
}

LexicalAnalyzer::~LexicalAnalyzer()
{
	tokens.clear();
	lexemes.clear();
	line_number.clear();
}

void LexicalAnalyzer::lexer(ifstream& sourceCode) {
	char c;
	string buffer;
	bool isComment = false;
	int line = 1;

	while (sourceCode.get(c)) {
		cout << c;	
	
		if (c == '!') { // check for comment block
			isComment = !isComment;
		}
		else {
			if (!isComment) {
				if (isOperator(c)) {
					if (buffer.length() > 0) {
						analyze(buffer, line);
						buffer.clear();
					}

					tokens.push_back("operator");
					lexemes.push_back(string(1, c));
					line_number.push_back(line);
				}
				else if (isSeparator(c) || c == '\n') {
					if (c == '.') buffer += c;
					else {
						if (buffer.length() > 0) {
							analyze(buffer, line);
							buffer.clear();
						}

						if (c != ' ' && c != '\n' && c != '\t') {
							tokens.push_back("separator");
							lexemes.push_back(string(1, c));
							line_number.push_back(line);
						}
					}
				}
				else {
					buffer += c;
				}
			}
		}	
		if (c == '\n') line++;
	}

	if (buffer.length() > 0)
		analyze(buffer, line);

	cout << endl;
	/*cout << endl << endl << setw(11) << "TOKENS" << "\tLEXEMES" << endl;
	for (int i = 0; i < tokens.size(); ++i) {
		cout << setw(11) << tokens[i] << "\t" << lexemes[i] << endl;
	}*/
}

void LexicalAnalyzer::lexer(string codeLine) {
	char c;
	string buffer;
	bool isComment = false;
	int line = 1;

	int i = 0;
	while (i < codeLine.length()) {
		c = codeLine[i];
		//cout << c;

		if (c == '!') { // check for comment block
			isComment = !isComment;
		}
		else {
			if (!isComment) {
				if (isOperator(c)) {
					if (buffer.length() > 0) {
						analyze(buffer, line);
						buffer.clear();
					}

					tokens.push_back("operator");
					lexemes.push_back(string(1, c));
					line_number.push_back(line);
				}
				else if (isSeparator(c) || c == '\n') {
					if (c == '.') buffer += c;
					else {
						if (buffer.length() > 0) {
							analyze(buffer, line);
							buffer.clear();
						}

						if (c != ' ' && c != '\n' && c != '\t') {
							tokens.push_back("separator");
							lexemes.push_back(string(1, c));
							line_number.push_back(line);
						}
					}
				}
				else {
					buffer += c;
				}
			}
		}
		//if (c == '\n') line++;
		i++;
	}

	if (buffer.length() > 0)
		analyze(buffer, line);

	/*cout << endl << endl << setw(11) << "TOKENS" << "\tLEXEMES" << endl;
	for (int i = 0; i < tokens.size(); ++i) {
	cout << setw(11) << tokens[i] << "\t" << lexemes[i] << endl;
	}*/
}

vector<string> LexicalAnalyzer::getTokens() {
	return tokens;
}

vector<string> LexicalAnalyzer::getLexemes() {
	return lexemes;
}

vector<int> LexicalAnalyzer::getLineNumber() {
	return line_number;
}

void LexicalAnalyzer::printResult(string fName) {
	ofstream writer(fName);
	writer << setw(11) << "TOKENS" << "\t" << "LEXEMES" << "\tLINE" << endl;
	for (int i = 0; i < tokens.size(); ++i) {
		writer << setw(11) << tokens[i] << "\t" << lexemes[i] << "\t" << line_number[i] << endl;
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

bool LexicalAnalyzer::isDigit(char ch) {
	return ch >= '0' && ch <= '9';
}

bool LexicalAnalyzer::isLetter(char ch) {
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

void LexicalAnalyzer::analyze(string word, int line) {
	if (isKeyword(word)) {
		tokens.push_back("keyword");
		lexemes.push_back(word);
		line_number.push_back(line);
		return;
	}

	// Traversing the FSM
	int state = 0;
	int col;
	string buffer;
	for (int i = 0; i < word.length(); ++i) {
		buffer += word[i];
		col = charToCol(word[i]);
		if (col != -1) // traverse the FSM if char is valid, skip the current char if invalid
			state = FSM[state][col];

		// handling special states (seperator '.')
		if (state == 6) { 			
			tokens.push_back("identifier");
			lexemes.push_back(string(buffer, 0, buffer.length()-1));
			line_number.push_back(line);

			tokens.push_back("seperator");
			lexemes.push_back(".");
			line_number.push_back(line);
			buffer.clear();
			state = 0;
		}
		if (state == 7) {
			tokens.push_back("seperator");
			lexemes.push_back(".");
			line_number.push_back(line);
			buffer.clear();
			i--;
			state = 0;
		}
		if (state == 8) {
			tokens.push_back("undefined");
			lexemes.push_back(string(buffer, 0, buffer.length() - 1));
			line_number.push_back(line);

			tokens.push_back("seperator");
			lexemes.push_back(".");
			line_number.push_back(line);
			buffer.clear();
			state = 0;
		}
		if (state == 9) {
			tokens.push_back("real");
			lexemes.push_back(string(buffer, 0, buffer.length() - 1));
			line_number.push_back(line);

			tokens.push_back("seperator");
			lexemes.push_back(".");
			line_number.push_back(line);
			buffer.clear();
			state = 0;
		}
	}

	// Adding items to tokens and lexemes lists
	switch (state) {			
		case 1:
			tokens.push_back("identifier");
			lexemes.push_back(buffer);
			line_number.push_back(line);
			break;

		case 2:
			tokens.push_back("int");
			lexemes.push_back(buffer);
			line_number.push_back(line);
			break;

		case 3:
			tokens.push_back("real");
			lexemes.push_back(buffer);
			line_number.push_back(line);
			break;

		case 4:
			tokens.push_back("seperator");
			lexemes.push_back(buffer);
			line_number.push_back(line);
			break;

		case 5:
			tokens.push_back("undefined");
			lexemes.push_back(buffer);
			line_number.push_back(line);
			break;

		default:
			break;
	}
}

int LexicalAnalyzer::charToCol(char ch) {
	if (isLetter(ch)) {
		return 0;
	}
	if (isDigit(ch)) {
		return 1;
	}
	if (ch == '$') {
		return 2;
	}
	if (ch == '.') {
		return 3;
	}
	return -1; // invalid char
}