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
		//cout << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		writer << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;

		if (statement(tokens, lexemes, line_number, i, writer)) {
		}
		else {
			writer.close();
			return;
		}
		++i;
		//cout << endl;
		writer << endl;
	} while (i < n);

	cout << endl << "Finished." << endl;
	writer << endl << "Finished." << endl;
	writer.close();

	cg.printCode();
}

bool SyntaxAnalyzer::nextToken(const vector<string>& tokens, const vector<string>& lexemes, int& i, ofstream& writer) {
	++i;
	if (i >= tokens.size()) {
		cout << "Error: end of token stream." << endl;
		writer << "Error: end of token stream." << endl;
		--i;
		return false;
	}
	if (i > prev_i) {
		//cout << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		writer << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		prev_i = i;
	}
	return true;
}

bool SyntaxAnalyzer::statement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {	
	//cout << "\t<Statement> -> <Expression-statement> | <Assign> | <If> | <Decalre> | <While>" << endl;
	writer << "\t<Statement> -> <Expression-statement> | <Assign> | <If> | <Declare> | <While>" << endl;

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
		else {
			cout << "Error: end of token stream." << endl;
			writer << "Error: end of token stream." << endl;
			return false;
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

	else if (lexemes[i] == "while"){ // while statemet
		if (whileStatement(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}
	// other statement


	else {
		cout << "Error: invalid statement - line " << line_number[i] << endl;
		writer << "Error: invalid statement - line " << line_number[i] << endl;
	}
	//writer.close();
	return false;
}

bool SyntaxAnalyzer::assign(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<Assign> -> identifier = <Expression>;" << endl;
	writer << "\t<Assign> -> identifier = <Expression>;" << endl;

	if (tokens[i] == "identifier") {
		string id = lexemes[i];
		int idLocation = cg.findSymbol(id);
		if (idLocation == -1) {
			cout << "Error: " << id << " is not declared - line " << line_number[i] << endl;
			return false;
		}

		if (!nextToken(tokens, lexemes, i, writer))  return false;
		if (lexemes[i] == "=") {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (expression(tokens, lexemes, line_number, i, writer)) {
				cg.generateCode("POPM", cg.getSymbolAddress(idLocation));

				if (!nextToken(tokens, lexemes, i, writer)) return false;
				if (lexemes[i] == ";") {
					return true;
				}
				else {
					cout << "Error: missing a \";\" - line " << line_number[i] << endl;
					writer << "Error: missing a \";\" - line " << line_number[i] << endl;
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
	//cout << "\t<Expression Statement> -> <Expression>;" << endl;
	writer << "\t<Expression Statement> -> <Expression>;" << endl;

	if (expression(tokens, lexemes, line_number, i, writer)) {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (lexemes[i] == ";") {
			return true;
		}
		else {
			cout << "Error: missing a \";\" - line " << line_number[i] << endl;
			writer << "Error: missing a \";\" - line " << line_number[i] << endl;
			return false;
		}
	}

	cout << "Error: invalid expression statement - line " << line_number[i] << endl;
	writer << "Error: invalid expression statement - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::expression(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<Expression> -> <Term> <ExpressionPrime>" << endl;
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
	//cout << "\t<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << endl;
	writer << "\t<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << endl;

	if (lexemes[i] == "+" || lexemes[i] == "-") {
		string op = lexemes[i];

		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (term(tokens, lexemes, line_number, i, writer)) {
			string command = (op == "+") ? "ADD" : "SUB";
			cg.generateCode(command, "nil");

			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (expressionPrime(tokens, lexemes, line_number, i, writer)) {
				return true;
			}
		}
	}
	else { //epsilon
		//cout << "\t<Empty> -> epsilon" << endl;
		writer << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}
	return false;
}

bool SyntaxAnalyzer::term(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<Term> -> <Factor> <TermPrime>" << endl;
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
	//cout << "\t<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;
	writer << "\t<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;

	if (lexemes[i] == "*" || lexemes[i] == "/") {
		string op = lexemes[i];

		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (factor(tokens, lexemes, line_number, i, writer)) {
			string command = (op == "*") ? "MUL" : "DIV";
			cg.generateCode(command, "nil");

			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (termPrime(tokens, lexemes, line_number, i, writer)) {
				return true;
			}
		}
	}
	else { //epsilon
		//cout << "\t<Empty> -> epsilon" << endl;
		writer << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}

	return false;
}

bool SyntaxAnalyzer::factor(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<Factor> -> - <Primary> | <Primary>" << endl;
	writer << "\t<Factor> -> - <Primary> | <Primary>" << endl;

	if (lexemes[i] == "-") {
		cg.generateCode("PUSHI", "0");

		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (primary(tokens, lexemes, line_number, i, writer)) {		
			cg.generateCode("SUB", "nil");
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
	//cout << "\t<Primary> -> identifier | int | ( <Expression> ) | real | true | false" << endl;
	writer << "\t<Primary> -> identifier | int | ( <Expression> ) | real | true | false" << endl;

	if (tokens[i] == "identifier" || tokens[i] == "int" || tokens[i] == "real" || lexemes[i] == "true" || lexemes[i] == "false") {
		if (tokens[i] == "identifier") {
			string id = lexemes[i];
			int idLocation = cg.findSymbol(id);
			if (idLocation == -1) {
				cout << "Error: " << id << " is not declared - line " << line_number[i] << endl;
				return false;
			}
			cg.generateCode("PUSHM", cg.getSymbolAddress(idLocation));
		}
		
		if (tokens[i] == "int" || tokens[i] == "real") 
			cg.generateCode("PUSHI", lexemes[i]);
		
		if (lexemes[i] == "true")
			cg.generateCode("PUSHI", "1");

		if (lexemes[i] == "false")
			cg.generateCode("PUSHI", "0");

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
				cout << "Error: missing a \")\" - line: " << line_number[i] << endl;
				writer << "Error: missing a \")\" - line: " << line_number[i] << endl;
				return false;
			}
		}
	}

	return false;
}

bool SyntaxAnalyzer::declare(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<Decalre> -> <Type> identifier <MoreIDs>;" << endl;
	writer << "\t<Decalre> -> <Type> identifier <MoreIDs>;" << endl;
	//cout << "\t<Type> -> int | float | bool" << endl;
	writer << "<Type> -> int | float | bool" << endl;

	if (lexemes[i] == "int" || lexemes[i] == "float" || lexemes[i] == "bool") { // <Type>
		cg.typeToAdd = lexemes[i];
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (tokens[i] == "identifier") {
			string id = lexemes[i];
			if (!cg.addSymbol(id, cg.typeToAdd)) {
				cout << "Error: " << lexemes[i] << " is already declared - line " << line_number[i] << endl;
				return false;
			}

			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (moreIDs(tokens, lexemes, line_number, i, writer)) {
				if (!nextToken(tokens, lexemes, i, writer)) return false;
				if (lexemes[i] == ";") {
					return true;
				}
				else {
					cout << "Error: missing a \";\" - line " << line_number[i] << endl;
					writer << "Error: missing a \";\" - line " << line_number[i] << endl;
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
	//cout << "\t<MoreIDs> -> , id <MoreIDs> | <Empty>" << endl;
	writer << "\t<MoreIDs> -> , id <MoreIDs> | <Empty>" << endl;

	if (lexemes[i] == ",") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (tokens[i] == "identifier") {
			string id = lexemes[i];
			if (!cg.addSymbol(id, cg.typeToAdd)) {
				cout << "Error: " << lexemes[i] << " variable is already declared - line" << line_number[i] << endl;
				return false;
			}

			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (moreIDs(tokens, lexemes, line_number, i, writer)) {
				return true;
			}
		}
	}
	else { // epsilon
		//cout << "\t<Empty> -> epsilon" << endl;
		writer << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}
	return false;
}

bool SyntaxAnalyzer::conditional(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<Conditional> -> <Expression> <ConditionalPrime> " << endl;
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
	//cout << "\t<ConditionalPrime> -> <Relop> <Expression> | <Empty>" << endl;
	writer << "\t<ConditionalPrime> -> <Relop> <Expression> | <Empty>" << endl;

	if (relop(tokens, lexemes, line_number, i, writer)) {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (expression(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}
	else{ // epsilon
		//cout << "\t<Empty> -> epsilon" << endl;
		writer << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}

	return false;
}

bool SyntaxAnalyzer::relop(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<Relop> -> < | <= | == | <> | >= | >" << endl;
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

	cout << "Error: invalid relational operator - line " << line_number[i] << endl;
	writer << "Error: invalid relational operator - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::ifStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<If> -> if <Conditional> then <StatementBlock> <ElseBlock> endif" << endl;
	writer << "\t<If> -> if <Conditional> then <StatementBlock> <ElseBlock> endif" << endl;

	if (lexemes[i] == "if") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (conditional(tokens, lexemes, line_number, i, writer)) {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (lexemes[i] == "then") {
				if (!nextToken(tokens, lexemes, i, writer)) return false;
				if (statementBlock(tokens, lexemes, line_number, i, writer)) {
					if (!nextToken(tokens, lexemes, i, writer)) return false;
					if (elseBlock(tokens, lexemes, line_number, i, writer)) {
						if (!nextToken(tokens, lexemes, i, writer)) {
							cout << "Error: missing \"endif\" - line " << line_number[i] << endl;
							writer << "Error: missing \"endif\" - line " << line_number[i] << endl;
							return false;
						}
						if (lexemes[i] == "endif") {
							return true;
						}
						else {
							cout << "Error: missing \"endif\" - line " << line_number[i] << endl;
							writer << "Error: missing \"endif\" - line " << line_number[i] << endl;
							return false;
						}
					}
				}
			}
			else {
				cout << "Error: missing \"then\" - line " << line_number[i] << endl;
				writer << "Error: missing \"then\" - line " << line_number[i] << endl;
				return false;
			}
		}
	}

	cout << "Error: invalid If block - line " << line_number[i] << endl;
	writer << "Error: invalid If block - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::statementBlock(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<StatementBlock> -> { <Statement> <moreStatement>" << endl;
	writer << "\t<StatementBlock> -> { <Statement> <moreStatement>" << endl;

	if (lexemes[i] == "{") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (statement(tokens, lexemes, line_number, i, writer)) {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (moreStatement(tokens, lexemes, line_number, i, writer)) {
				return true;
			}
		}
	}

	cout << "Error: invalid { statement block } - line " << line_number[i] << endl;
	writer << "Error: invalid { statement block } - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::moreStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<moreStatement> -> <Statement> <moreStatement> | }" << endl;
	writer << "\t<moreStatement> -> <Statement> <moreStatement> | }" << endl;

	if (lexemes[i] == "}") {
		return true;
	}
	else {
		if (statement(tokens, lexemes, line_number, i, writer)) {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (moreStatement(tokens, lexemes, line_number, i, writer)) {
				return true;
			}
		}
		else return false;
	}

	cout << "Error: missing a \"}\" - line " << line_number[i] << endl;
	writer << "Error: missing a \"}\" - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::elseBlock(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<ElseBlock> -> else <StatementBlock> | <Empty>" << endl;
	writer << "\t<ElseBlock> -> else <StatementBlock> | <Empty>" << endl;

	if (lexemes[i] == "else") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (statementBlock(tokens, lexemes, line_number, i, writer)) {
			return true;
		}
	}
	else { // epsilon
		//cout << "\t<Empty> -> epsilon" << endl;
		writer << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}

	return false;
}

bool SyntaxAnalyzer::whileStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer) {
	//cout << "\t<While> -> while <Conditional> do <StatementBlock> whileend" << endl;
	writer << "\t<While> -> while <Conditional> do <StatementBlock> whileend" << endl;

	if (lexemes[i] == "while") {
		if (!nextToken(tokens, lexemes, i, writer)) return false;
		if (conditional(tokens, lexemes, line_number, i, writer)) {
			if (!nextToken(tokens, lexemes, i, writer)) return false;
			if (lexemes[i] == "do") {
				if (!nextToken(tokens, lexemes, i, writer)) return false;
				if (statementBlock(tokens, lexemes, line_number, i, writer)) {
					if (!nextToken(tokens, lexemes, i, writer)) {
						cout << "Error: missing \"whileend\" - line " << line_number[i] << endl;
						writer << "Error: missing whileend - line " << line_number[i] << endl;
						return false;
					}
					if (lexemes[i] == "whileend") {
						return true;
					}
					else {
						cout << "Error: missing \"whileend\" - line " << line_number[i] << endl;
						writer << "Error: missing \"whileend\" - line " << line_number[i] << endl;
						return false;
					}
				}
			}
			else {
				cout << "Error: missing do block - line " << line_number[i] << endl;
				writer << "Error: missing do block - line " << line_number[i] << endl;
				return false;
			}
		}
	}

	cout << "Error: invalid While block - line " << line_number[i] << endl;
	writer << "Error: invalid While block - line " << line_number[i] << endl;
	return false;
}