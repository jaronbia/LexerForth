/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#pragma once
#include <fstream>
#include <string>
#include <map>
#include <iostream>

using namespace std;

// enum TokenT { WORD, STRING, START, COMMENT, COMMENTBLK, SYMBOL };
enum TokenT { NUMBER, WORD, STRINGS, COMMENT, COMMENTBLK };
enum State { START, SYMBOL, STRINGSs, TOKEN };

class Token {
private:
    int occur;      // Number of occurrences 
    string name;    // Name of token
    TokenT type;

public:
    Token(int occ = 0, string n = "", TokenT t = NUMBER) : occur(occ), name(n), type(t) {};
    ~Token() = default;

    const bool compare(const Token& t) const { return name == t.name; }
};

inline bool operator == (const Token& t1, const Token& t2) { return t1.compare(t2); }

class Lexer {
private:
    ifstream in;
    ofstream out;
    map<Token, int> symbolTable;
    string temp;
    TokenT Ttype = NUMBER;
    bool endParen = true;

    void startLex(string& line, State& st);
    void acquireSymbol(string& line, State& st);
    void acquireString(string& line, State& st);
    void foundToken(string& line, State& st);

public:
    Lexer(string filename);
    ~Lexer() = default;
    ostream& print(ostream& out);
    void lex();
};
inline ostream& operator << (ostream& out, Lexer& lr) { return lr.print(out); }