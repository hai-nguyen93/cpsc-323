#pragma once
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer();
	SyntaxAnalyzer(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number);
	~SyntaxAnalyzer();

	void analyze(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number);
	bool nextToken(const vector<string>& tokens, const vector<string>& lexemes, int &i, ofstream& writer);

	bool statement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool assign(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool expressionStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool declare(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool ifStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool statementBlock(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool moreStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool elseBlock(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool whileStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);

	bool expression(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool expressionPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool term(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool termPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool factor(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool primary(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);	
	bool moreIDs(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool conditional(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool conditionalPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	bool relop(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& writer);
	

private:
	int prev_i = -1;
};

