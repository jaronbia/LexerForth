/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>

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

        const bool equals(const Token& t) const { return name == t.name; }
};

inline bool operator == (const Token& t1, const Token& t2) { return t1.equals(t2); }

//-------------------------------------------------------------------------

enum State { START, SYMBOL, STRING, TOKEN };

class Lexer {
    private:
        ifstream in;
        ofstream out;
        //map<Token, int> symbolTable;

        void startLex(string& line, State& st);
        void acquireSymbol(string& line, State& st);
        void acquireString(string& line, State& st);
        void foundToken(string& line, State& st);

        void readBlkComment(string& line, int& j);

    public:
        Lexer(string filename);
        ~Lexer() = default;

        void lex();
        ostream& print(ostream& out) { return out; }
};

inline ostream& operator << (ostream& out, Lexer& lr) { return lr.print(out); }