#include "SyntaxAnalyzer.h"
#include <iostream>
#include <fstream>


SyntaxAnalyzer::SyntaxAnalyzer()
{
}

SyntaxAnalyzer::SyntaxAnalyzer(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number)
{
	analyze(tokens, lexemes, line_number);
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
}

void SyntaxAnalyzer::analyze(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number) {
	ofstream writer("parse_result.txt");

	int n = tokens.size();
	int i = 0;

	do {
		cout << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		cout << "\t<Statement> -> <Compound> | <Expression-statement> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>" << endl;
		writer << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		writer << "\t<Statement> -> <Compound> | <Expression-statement> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>" << endl;

		if (tokens[i] == "identifier") { // assign or expression statement
			if (!nextToken(tokens, lexemes, i, writer))  return;
			if (assign(tokens, lexemes, line_number, i, writer)) {
				//++i;
				//continue;
			}
			else if (expressionStatement(tokens, lexemes, line_number, i, writer)) {
				//++i;
				//continue;
			}
			else {
				cout << "Error: invalid statement - line " << line_number[i] << endl;
				writer << "Error: invalid statement - line " << line_number[i] << endl;
				writer.close();
				return;
			}
		}

		else if (tokens[i] == "int" || tokens[i] == "real") { // expression statement
			if (expressionStatement(tokens, lexemes, line_number, i, writer)) {
				//++i;
				//continue;
			}
			else {
				cout << "Error: invalid statement - line " << line_number[i] << endl;
				writer << "Error: invalid statement - line " << line_number[i] << endl;
				writer.close();
				return;
			}
		}

		// other statements
		++i;
		cout << endl;
	} while (i < n);

	cout << endl << "Finished." << endl;
	writer << endl << "Finished." << endl;
	writer.close();
}

bool SyntaxAnalyzer::nextToken(const vector<string>& tokens, const vector<string>& lexemes, int& i, ofstream& writer) {
	++i;
	if (i >= tokens.size()) {
		cout << "End of token stream." << endl;
		writer << "End of token stream." << endl;
		--i;
		return false;
	}
	if (i > prev_i) {
		cout << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		writer << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		prev_i = i;
	}
	return true;
}

bool SyntaxAnalyzer::assign(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Assign> -> identifier = <Expression>;" << endl;
	writer << "\t<Assign> -> identifier = <Expression>;" << endl;

	if (lexemes[i] == "=") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (expression(tokens, lexemes, line_number, i, writer)) {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (lexemes[i] == ";") {
				return true;
			}
			else {
				cout << "Error: expect a \";\" - line " << line_number[i] << endl;
				writer << "Error: expect a \";\" - line " << line_number[i] << endl;
				return false;
			}
		}
	}
	cout << "Error: invalid assignment statement - line " << line_number[i] << endl;
	writer << "Error: invalid assignment statement - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::expressionStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer){
	// TODO
	return false;
}

bool SyntaxAnalyzer::expression(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Expression> -> <Term> <ExpressionPrime>" << endl;
	writer << "\t<Expression> -> <Term> <ExpressionPrime>" << endl;

	if (term(tokens, lexemes, line_number, i, writer)) {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (expressionPrime(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}
	cout << "Error: invalid expression - line " << line_number[i] << endl;
	writer << "Error: invalid expression - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::expressionPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << endl;
	writer << "\t<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << endl;

	if (lexemes[i] == "+" || lexemes[i] == "-") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (term(tokens, lexemes, line_number, i, writer)) {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (expressionPrime(tokens, lexemes, line_number, i, writer)) {
				return true;
			}
		}
	}
	else { //epsilon
		cout << "\t<Empty> -> epsilon" << endl;
		writer << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}
	cout << "Error: invalid expression prime - line " << line_number[i] << endl;
	writer << "Error: invalid expression prime - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::term(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Term> -> <Factor> <TermPrime>" << endl;
	writer << "\t<Term> -> <Factor> <TermPrime>" << endl;

	if (factor(tokens, lexemes, line_number, i, writer)) {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (termPrime(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}

	cout << "Error: invalid term - line " << line_number[i] << endl;
	writer << "Error: invalid term - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::termPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;
	writer << "\t<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;

	if (lexemes[i] == "*" || lexemes[i] == "/") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (factor(tokens, lexemes, line_number, i, writer)) {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (termPrime(tokens, lexemes, line_number, i, writer)) {
				return true;
			}
		}
	}
	else { //epsilon
		cout << "\t<Empty> -> epsilon" << endl;
		writer << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}

	cout << "Error: invalid term prime - line: " << line_number[i] << endl;
	writer << "Error: invalid term prime - line: " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::factor(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Factor> -> - <Primary> | <Primary>" << endl;
	writer << "\t<Factor> -> - <Primary> | <Primary>" << endl;

	if (tokens[i] == "-") {
		//cout << "\t<Factor> -> - <Primary>" << endl;
		//writer << "\t<Factor> -> - <Primary>" << endl;
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (primary(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}
	else { 		
		//cout << "\t<Factor> -> <Primary>" << endl;				
		//writer << "\t<Factor> -> <Primary>" << endl;
		if (primary(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}
	cout << "Error: invalid factor - line: " << line_number[i] << endl;
	writer << "Error: invalid factor - line: " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::primary(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Primary> -> identifier | int | identifier ( <IDs> ) | ( <Expression> ) | real | true | false" << endl;
	writer << "\t<Primary> -> identifier | int | identifier ( <IDs> ) | ( <Expression> ) | real | true | false" << endl;

	if (tokens[i] == "identifier" || tokens[i] == "int" || tokens[i] == "real") {
		return true;
	}
	if (lexemes[i] == "(") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (expression(tokens, lexemes, line_number, i, writer)) {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (lexemes[i] == ")") {
				return true;
			}
			else {
				cout << "Error: expect a \")\" - line: " << line_number[i] << endl;
				writer << "Error: expect a \")\" - line: " << line_number[i] << endl;
				return false;
			}
		}
	}

	cout << "Error: invalid primary - line: " << line_number[i] << endl;
	writer << "Error: invalid primary - line: " << line_number[i] << endl;
	return false;
}