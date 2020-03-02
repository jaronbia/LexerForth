#pragma once
#include <fstream>
#include <string>
#include <map>
#include <iostream>

using namespace std;

enum TokenT { WORD, STRING, START, COMMENT, COMMENTBLK };

class Token {
private:
    int occur;      // Number of occurrences 
    string name;    // Name of token
    TokenT type;

public:
    Token(int occ = 0, string n = "", TokenT t = START) : occur(occ), name(n), type(t) {};
    ~Token() = default;

    const bool compare(const Token& t) const { return name == t.name; }
};

inline bool operator == (const Token& t1, const Token& t2) { return t1.compare(t2); }

class Lexer {
private:
    ifstream in;
    ofstream out;
    map<Token, int> symbolTable;

    void startLex();
    void acquireSymbol();
    void acquireString();
    void foundToken();

public:
    Lexer(string filename);
    ~Lexer() = default;
    ostream& print(ostream& out) { return out; }
    void lex();
};
inline ostream& operator << (ostream& out, Lexer& lr) { return lr.print(out); }