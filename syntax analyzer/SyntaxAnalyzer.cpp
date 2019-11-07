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
		writer << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;

		if (statement(tokens, lexemes, line_number, i, writer)) {
		}
		else {
			return;
		}
		++i;
		cout << endl;
		writer << endl;
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

bool SyntaxAnalyzer::statement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {	
	cout << "\t<Statement> -> <Expression-statement> | <Assign> | <If> | <Decalre> | <Return> | <While>" << endl;
	writer << "\t<Statement> -> <Expression-statement> | <Assign> | <If> | <Declare> | <Return> | <While>" << endl;

	int startIndex = i;  // first token of a statement
	if (tokens[i] == "identifier") { // assign or expression statement
		if (i + 1 < tokens.size()) {
			if (lexemes[i + 1] == "=") {  // assign
				if (assign(tokens, lexemes, line_number, i, writer)) {
					return true;
				}
			}
			else { // expression statement
				//i = startIndex;
				if (expressionStatement(tokens, lexemes, line_number, i, writer)) {
					return true;
				}
			}
		}
	}

	else if (tokens[i] == "int" || tokens[i] == "real" || lexemes[i] == "(" || lexemes[i] == "-") { // expression statement
		if (expressionStatement(tokens, lexemes, line_number, i, writer)) {
			return true;
		}		
	}

	else if (lexemes[i] == "int" || lexemes[i] == "float" || lexemes[i] == "bool") { // declare statement
		if (declare(tokens, lexemes, line_number, i, writer)) {
			return true;
		}		
	}

	else if (lexemes[i] == "if") { // if statement
		if (ifStatement(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}

	// other statement


	//cout << "Error: invalid statement - line " << line_number[i] << endl;
	//writer << "Error: invalid statement - line " << line_number[i] << endl;
	writer.close();
	return false;
}

bool SyntaxAnalyzer::assign(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Assign> -> identifier = <Expression>;" << endl;
	writer << "\t<Assign> -> identifier = <Expression>;" << endl;

	if (tokens[i] == "identifier") {
		if (!nextToken(tokens, lexemes, i, writer))  return false;
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
	}

	cout << "Error: invalid assign statement - line " << line_number[i] << endl;
	writer << "Error: invalid assign statement - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::expressionStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer){
	cout << "\t<Expression Statement> -> <Expression>;" << endl;
	writer << "\t<Expression Statement> -> <Expression>;" << endl;

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

	cout << "Error: invalid expression statement - line " << line_number[i] << endl;
	writer << "Error: invalid expression statement - line " << line_number[i] << endl;
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

	return false;
}

bool SyntaxAnalyzer::factor(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Factor> -> - <Primary> | <Primary>" << endl;
	writer << "\t<Factor> -> - <Primary> | <Primary>" << endl;

	if (lexemes[i] == "-") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (primary(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}
	else { 		
		if (primary(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}

	return false;
}

bool SyntaxAnalyzer::primary(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Primary> -> identifier | int | ( <Expression> ) | real | true | false" << endl;
	writer << "\t<Primary> -> identifier | int | ( <Expression> ) | real | true | false" << endl;

	if (tokens[i] == "identifier" || tokens[i] == "int" || tokens[i] == "real" || lexemes[i] == "true" || lexemes[i] == "false") {
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

	return false;
}

bool SyntaxAnalyzer::declare(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Decalre> -> <Type> identifier <MoreIDs>;" << endl;
	writer << "\t<Decalre> -> <Type> identifier <MoreIDs>;" << endl;

	if (lexemes[i] == "int" || lexemes[i] == "float" || lexemes[i] == "bool") { // <Type>
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (tokens[i] == "identifier") {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (moreIDs(tokens, lexemes, line_number, i, writer)) {
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
	}

	cout << "Error: invalid declare statement - line " << line_number[i] << endl;
	writer << "Error: invalid declare statement - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::moreIDs(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<MoreIDs> -> , id <MoreIDs> | <Empty>" << endl;
	writer << "\t<MoreIDs> -> , id <MoreIDs> | <Empty>" << endl;

	if (lexemes[i] == ",") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (tokens[i] == "identifier") {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (moreIDs(tokens, lexemes, line_number, i, writer)) {
				return true;
			}
		}
	}
	else { // epsilon
		cout << "\t<Empty> -> epsilon" << endl;
		writer << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}
	return false;
}

bool SyntaxAnalyzer::conditional(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Conditional> -> <Expression> <ConditionalPrime> " << endl;
	writer << "\t<Conditional> -> <Expression> <ConditionalPrime>" << endl;

	if (expression(tokens, lexemes, line_number, i, writer)){
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (conditionalPrime(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}
	return false;
}

bool SyntaxAnalyzer::conditionalPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<ConditionalPrime> -> <Relop> <Expression> | <Empty>" << endl;
	writer << "\t<ConditionalPrime> -> <Relop> <Expression> | <Empty>" << endl;

	if (relop(tokens, lexemes, line_number, i, writer)) {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (expression(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}
	else{ // epsilon
		cout << "\t<Empty> -> epsilon" << endl;
		writer << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}

	return false;
}

bool SyntaxAnalyzer::relop(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<Relop> -> < | <= | == | <> | >= | >" << endl;
	writer << "\t<Relop> -> < | <= | == | <> | >= | >" << endl;

	if (lexemes[i] == "<") { // < or <= or <>
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (lexemes[i] == "=") // <= 
			return true;
		else if (lexemes[i] == ">") // <>
			return true;
		else{ // <			
			--i;
			return true;
		}	
	}
	if (lexemes[i] == "=") { // ==
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (lexemes[i] == "=")
			return true;
	}
	if (lexemes[i] == ">") { // > or >=
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (lexemes[i] == "=") // >=
			return true;
		else{ // >
			--i;
			return true;
		}
	}

	return false;
}

bool SyntaxAnalyzer::ifStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	cout << "\t<If> -> if <Conditional> then <Statement> endif" << endl;
	writer << "\t<If> -> if <Conditional> then <Statement> endif" << endl;

	if (lexemes[i] == "if") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (conditional(tokens, lexemes, line_number, i, writer)) {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (lexemes[i] == "then") {
				if (!nextToken(tokens, lexemes, i, writer)) return false;
				if (statement(tokens, lexemes, line_number, i, writer)) {
					if (!nextToken(tokens, lexemes, i, writer)) return false;
					if (lexemes[i] == "endif") {
						return true;
					}
				}
			}
		}
	}

	cout << "Error: invalid If block - line " << line_number[i] << endl;
	writer << "Error: invalid If block - line " << line_number[i] << endl;
	return false;
}